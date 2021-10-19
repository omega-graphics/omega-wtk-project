#include "Compositor.h"
#include "omegaWTK/Composition/Layer.h"
#include <chrono>
#include <mutex>
namespace OmegaWTK::Composition {

//void Compositor::hasDetached(LayerTree *tree){
//    for(auto it = targetLayerTrees.begin();it != targetLayerTrees.end();it++){
//        if(tree == *it){
//            targetLayerTrees.erase(it);
//            renderTargetStore.cleanTreeTargets(tree);
//            tree->removeObserver(this);
//            break;
//        }
//    }
//};

void CompositorScheduler::processCommand(SharedHandle<CompositorCommand> & command ){
    auto _now = std::chrono::high_resolution_clock::now();

    if(command->thresholdParams.hasThreshold) {
        if(command->thresholdParams.threshold >= _now){
            /// Command will execute on time.
            auto command = compositor->commandQueue.first();
            compositor->commandQueue.pop();

            compositor->currentCommand = command;
            
            std::chrono::nanoseconds diff = command->thresholdParams.threshold - command->thresholdParams.timeStamp;
            std::this_thread::sleep_for(diff);

            compositor->executeCurrentCommand();
            
        }
        else {
            // Command is late!!
            auto command = compositor->commandQueue.first();
            compositor->currentCommand = command;
            compositor->commandQueue.pop();
            compositor->executeCurrentCommand();
        };
    }
    else {
        /// Command will be executed right away.
        auto command = compositor->commandQueue.first();
        compositor->currentCommand = command;
        compositor->commandQueue.pop();
        compositor->executeCurrentCommand();
    }
};




CompositorScheduler::CompositorScheduler(Compositor * compositor):compositor(compositor),shutdown(false),t([&](){
        std::cout << "--> Starting Up" << std::endl;
        while(true){
            {
                bool hasCommands = false;
                {
                    std::unique_lock<std::mutex> lk(compositor->mutex);
                    compositor->queueCondition.wait(lk,[&]{ return !compositor->commandQueue.empty();});
                    lk.unlock();
                }
                
                auto & command = compositor->commandQueue.first();
                processCommand(command);
            
            }
            {
                std::lock_guard<std::mutex> shutdown_lk(compositor->mutex);
                if(shutdown){
                    break;
                };
            }
        };

        {
            std::lock_guard<std::mutex> lk(compositor->mutex);
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
        std::lock_guard<std::mutex> lk(compositor->mutex); 
        shutdown = true;
    }
    t.join();
};


Compositor::Compositor():queueIsReady(false),queueCondition(),commandQueue(200),scheduler(this){
    
};

Compositor::~Compositor(){
     std::cout << "~Compositor" << std::endl;
};

void Compositor::scheduleCommand(SharedHandle<CompositorCommand> & command){

    {
        std::unique_lock<std::mutex> lk(mutex);
        commandQueue.push(command);
    }
    queueCondition.notify_one();
};


};
