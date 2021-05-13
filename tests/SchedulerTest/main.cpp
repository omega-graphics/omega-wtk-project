#include <OmegaWTK.h>
#include <chrono>
#include <thread>

using namespace OmegaWTK;


struct CompareRenderCommands {
    auto operator()(Composition::CompositionRenderCommand & lhs,Composition::CompositionRenderCommand & rhs){
        bool cond1,cond2;
        cond1 = lhs.thresholdParams.timeStamp > rhs.thresholdParams.timeStamp;
        cond2 = lhs.thresholdParams.hasThreshold? (rhs.thresholdParams.hasThreshold? (lhs.thresholdParams.threshold < rhs.thresholdParams.threshold) : true) : rhs.thresholdParams.hasThreshold? (lhs.thresholdParams.hasThreshold? (rhs.thresholdParams.threshold < lhs.thresholdParams.threshold) : false)  : true;
        // printf("Sort Cond:%i\n",((cond1 == true) && (cond2 == true)));
        return (cond1 == true) && (cond2 == true);
    };
};



static Core::PriorityQueueHeap<Composition::CompositionRenderCommand,CompareRenderCommands> queue(10);
std::mutex queue_mutex;
std::mutex shutdown_mutex;
bool shutdown_;

void processCommand(Composition::CompositionRenderCommand & command ){
    auto _now = std::chrono::high_resolution_clock::now();
    if(command.thresholdParams.hasThreshold) {
        if(command.thresholdParams.threshold >= _now){
            auto command = queue.first();
            queue.pop();
            std::string *s = (std::string *)command.data;
            std::cout << *s << std::endl;
            delete s;
        };
    }
    else {
        /// Do Whenever!
        auto command = queue.first();
        queue.pop();
        std::string *s = (std::string *)command.data;
        std::cout << *s << std::endl;
        delete s;
    }
};

static void run(){
    std::cout << "--> Starting Up" << std::endl;
    while(!shutdown_){
        {
            std::lock_guard<std::mutex> lk(queue_mutex);
            if(!queue.empty()){
                auto & command = queue.first();
                processCommand(command);
            };
        }
        std::lock_guard<std::mutex> shutdown_lk(shutdown_mutex);
    };

    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        if(!queue.empty()){
            std::cout << "--> Unfinished Jobs:" << queue.length() << std::endl;
        };
    }
    
    std::cout << "--> Shutting Down" << std::endl;
};

int main(int argc,char *argv[]){
    std::cout << "Start" << std::endl;
    shutdown_ = false;
    
    Composition::CompositionRenderCommand renderCommand;
    renderCommand.data = new std::string("--> Low Priority Frame 1 Has Finished");
    renderCommand.thresholdParams.hasThreshold = false;
    auto time_stamp_0 = std::chrono::high_resolution_clock::now();
    renderCommand.thresholdParams.timeStamp = time_stamp_0;
    
    // std::lock_guard<std::mutex> lk(queue_mutex);
    queue.push(renderCommand);
    std::cout << "Command Push 1" << std::endl;
    

    Composition::CompositionRenderCommand renderCommand2;
    renderCommand2.data = new std::string("--> High Priority Frame 2 Has Finished");
    renderCommand2.thresholdParams.hasThreshold = true;
    auto time_stamp_1 = std::chrono::high_resolution_clock::now();
    renderCommand2.thresholdParams.timeStamp = time_stamp_1;
    renderCommand2.thresholdParams.threshold = time_stamp_1 + std::chrono::milliseconds(15);
    
    // std::lock_guard<std::mutex> lk(queue_mutex);
    queue.push(renderCommand2);
    std::cout << "Command Push 2" << std::endl;
    

    std::thread t1(&run);

    auto time_stamp_2 = std::chrono::high_resolution_clock::now();
    renderCommand2.data = new std::string("--> High Priority Frame 3 Has Finished");
    renderCommand2.thresholdParams.timeStamp = time_stamp_2;
    renderCommand2.thresholdParams.threshold = time_stamp_2 + std::chrono::milliseconds(15);

    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        queue.push(renderCommand2);
    }

    std::this_thread::sleep_for(std::chrono::seconds(4));
    
    {
        
        std::lock_guard<std::mutex> shutdown_lk(shutdown_mutex);
        shutdown_ = true;
         std::cout << "Shutdown" << std::endl;
    }


    t1.join();

    return 0;
};