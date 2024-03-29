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
            compositor->currentCommand = command;
            compositor->commandQueue.pop();
            
            
             std::this_thread::sleep_until(command->thresholdParams.threshold);
            

            compositor->executeCurrentCommand();
            
        }
        else {
            // Command is late!!
            compositor->currentCommand = command;
            compositor->commandQueue.pop();
            compositor->executeCurrentCommand();
        };
    }
    else {
        /// Command will be executed right away.
        compositor->currentCommand = command;
        compositor->commandQueue.pop();
        compositor->executeCurrentCommand();
    }
};




CompositorScheduler::CompositorScheduler(Compositor * compositor):compositor(compositor),shutdown(false),t([this](Compositor *compositor){
//        std::cout << "--> Starting Up" << std::endl;
        while(true){
            {
                {
                    std::unique_lock<std::mutex> lk(compositor->mutex);
                    compositor->queueCondition.wait(lk,[&]{ return !compositor->commandQueue.empty();});
                    lk.unlock();
                }

                {
                    std::lock_guard<std::mutex> lk(compositor->mutex);
                    auto command = compositor->commandQueue.first();
                    processCommand(command);
                }
            
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
    },compositor){

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
     std::cout << "~Compositor()" << std::endl;
};

void Compositor::scheduleCommand(SharedHandle<CompositorCommand> & command){
    {
        std::lock_guard<std::mutex> lk(mutex);
        commandQueue.push(std::move(command));
    }
    queueCondition.notify_one();
};


};
