#include "Compositor.h"
#include "omegaWTK/Composition/Layer.h"
#include <chrono>
#include <mutex>
namespace OmegaWTK::Composition {

void Compositor::hasDetached(LayerTree *tree){
    for(auto it = targetLayerTrees.begin();it != targetLayerTrees.end();it++){
        if(tree == *it){
            targetLayerTrees.erase(it);
            renderTargetStore.cleanTreeTargets(tree);
            tree->removeObserver(this);
            break;
        }
    }
};

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




CompositorScheduler::CompositorScheduler(Compositor * compositor):compositor(compositor),shutdown(false),mutex(),t([&](){
        std::cout << "--> Starting Up" << std::endl;
        while(true){
            {
                bool hasCommands = false;
                // {
                //     std::unique_lock<std::mutex> lk(compositor->queueMutex);
                //     compositor->queueCondition.wait(lk,[&]{ return compositor->queueIsReady;});
                //     hasCommands = !compositor->commandQueue.empty();
                //     lk.unlock();
                // }

                if(hasCommands){
                    auto & command = compositor->commandQueue.first();
                    processCommand(command);
                };
            }
            {
                std::lock_guard<std::mutex> shutdown_lk(mutex);
                if(shutdown){
                    break;
                };
            }
        };

        {
            std::lock_guard<std::mutex> lk(*compositor->queueMutex);
            if(!compositor->commandQueue.empty()){
                std::cout << "--> Unfinished Jobs:" << compositor->commandQueue.length() << std::endl;
            };
        }
        
        std::cout << "--> Shutting Down" << std::endl;
    }){

};

CompositorScheduler::~CompositorScheduler(){
    std::cout << "close" << std::endl;
    {
        std::lock_guard<std::mutex> lk(mutex); 
        shutdown = true;
    }
    t.join();
};


Compositor::Compositor():queueMutex(new std::mutex()),queueIsReady(false),queueCondition(),commandQueue(200),scheduler(this){
    
};

Compositor::~Compositor(){
    std::cout << std::endl;
     std::cout << "~Compositor" << std::endl;
     delete queueMutex;
};

void Compositor::scheduleCommand(CompositionRenderCommand * command){
    auto ptr = command;

    {
        
        std::cout << "BEFORE LOCK (pthread_mutex_ptr :" << queueMutex->native_handle() << ")" << std::endl;
        
        // if (!queueMutex.try_lock()){
        //     std::cout << "LOCK IS OWNED BY OTHER THREAD!" << std::endl;
        // };
        std::unique_lock<std::mutex> lk(*queueMutex);
        std::cout << "AFTER LOCK" << std::endl;
        queueIsReady = false;
        commandQueue.push(*ptr);
        queueIsReady = true;
        std::cout << "PUSHED COMMAND" << std::endl;
        lk.unlock();
    }
    // queueCondition.notify_one();
};


};
