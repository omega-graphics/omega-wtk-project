#include "Compositor.h"
#include "omegaWTK/Composition/Layer.h"
#include <chrono>
namespace OmegaWTK::Composition {



void CompositorScheduler::processCommand(CompositionRenderCommand & command ){
    auto _now = std::chrono::high_resolution_clock::now();
    if(command.thresholdParams.hasThreshold) {
        if(command.thresholdParams.threshold >= _now){
            /// Command will execute on time.
            auto command = compositor->commandQueue.first();
            compositor->commandQueue.pop();

            compositor->currentCommand = &command;
            
            std::chrono::nanoseconds diff = command.thresholdParams.threshold - command.thresholdParams.timeStamp;
            std::this_thread::sleep_for(diff);

            auto future_status = compositor->executeCurrentRenderCommand();
            
        }
        else {
            // Command is late!!
            auto command = compositor->commandQueue.first();
            compositor->currentCommand = &command;
            compositor->commandQueue.pop();
            auto future_status = compositor->executeCurrentRenderCommand();
        };
    }
    else {
        /// Command will be executed right away.
        auto command = compositor->commandQueue.first();
        compositor->currentCommand = &command;
        compositor->commandQueue.pop();
        auto future_status = compositor->executeCurrentRenderCommand();
    }
};




CompositorScheduler::CompositorScheduler(Compositor * compositor):compositor(compositor),shutdown(false){
    run();
};

void CompositorScheduler::run() {
    t = new std::thread([&](){
        std::cout << "--> Starting Up" << std::endl;
        while(!shutdown){
            {
                std::lock_guard<std::mutex> lk(compositor->queueMutex);
                if(!compositor->commandQueue.empty()){
                    auto & command = compositor->commandQueue.first();
                    processCommand(command);
                };
            }
            std::lock_guard<std::mutex> shutdown_lk(mutex);
        };

        {
            std::lock_guard<std::mutex> lk(compositor->queueMutex);
            if(!compositor->commandQueue.empty()){
                std::cout << "--> Unfinished Jobs:" << compositor->commandQueue.length() << std::endl;
            };
        }
        
        std::cout << "--> Shutting Down" << std::endl;
    });
};

CompositorScheduler::~CompositorScheduler(){
    {
        std::lock_guard<std::mutex> lk(mutex); 
        shutdown = true;
    }
    t->join();
    delete t;
};


Compositor::Compositor():commandQueue(200),scheduler(std::make_unique<CompositorScheduler>(this)){
    
};

void Compositor::scheduleCommand(UniqueHandle<CompositionRenderCommand> command){
    const std::lock_guard<std::mutex> lk(queueMutex);
    auto ptr = command.release();
    commandQueue.push(*ptr);
};


};
