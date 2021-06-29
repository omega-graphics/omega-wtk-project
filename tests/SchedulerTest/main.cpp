#include <OmegaWTK.h>
#include <chrono>
#include <thread>

using namespace OmegaWTK;


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