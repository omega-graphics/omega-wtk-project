#include <OmegaWTK.h>

using namespace OmegaWTK;

static Core::QueueHeap<Composition::CompositionRenderCommand> queue(10);
std::mutex queue_mutex;
std::mutex shutdown_mutex;
bool shutdown_;

static void run(){
    std::cout << "--> Starting Up" << std::endl;
    while(!shutdown_){
        {
            std::lock_guard<std::mutex> lk(queue_mutex);
            if(!queue.empty()){
                auto & command = queue.first();
                auto _now = std::chrono::high_resolution_clock::now();
                if(command.thresholdParams.threshold >= _now){
                    auto command = queue.first();
                    queue.pop();
                    std::string *s = (std::string *)command.data;
                    std::cout << *s << std::endl;
                    delete s;
                };
            };
        }
        std::lock_guard<std::mutex> shutdown_lk(shutdown_mutex);
    };

    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        if(!queue.empty()){
            std::cout << "Unfinished Jobs:" << queue.length() << std::endl;
        };
    }
    
    std::cout << "--> Shutting Down" << std::endl;
};

int main(int argc,char *argv[]){
    std::cout << "Start" << std::endl;
    shutdown_ = false;
    std::thread t1(&run);
    
    Composition::CompositionRenderCommand renderCommand;
    renderCommand.priority = OmegaWTK::Composition::CompositionRenderCommand::High;
    renderCommand.data = new std::string("--> High Priority Frame 1 Has Finished");
    renderCommand.thresholdParams.hasThreshold = true;
    auto time_stamp_0 = std::chrono::high_resolution_clock::now();
    renderCommand.thresholdParams.timeStamp = time_stamp_0;
    renderCommand.thresholdParams.threshold = time_stamp_0 + std::chrono::milliseconds(15);
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        queue.push(renderCommand);
        std::cout << "Command Push" << std::endl;
    }

    Composition::CompositionRenderCommand renderCommand2;
    renderCommand2.priority = OmegaWTK::Composition::CompositionRenderCommand::High;
    renderCommand2.data = new std::string("--> High Priority Frame 2 Has Finished");
    renderCommand2.thresholdParams.hasThreshold = true;
    auto time_stamp_1 = std::chrono::high_resolution_clock::now();
    renderCommand2.thresholdParams.timeStamp = time_stamp_1;
    renderCommand2.thresholdParams.timeStamp = time_stamp_1 + std::chrono::milliseconds(15);
    {
        std::lock_guard<std::mutex> lk(queue_mutex);
        queue.push(renderCommand2);
         std::cout << "Command Push" << std::endl;
    }

    {
        std::lock_guard<std::mutex> shutdown_lk(shutdown_mutex);
        shutdown_ = true;
         std::cout << "Shutdown" << std::endl;
    }


    t1.join();

    return 0;
};