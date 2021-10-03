#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/CompositorClient.h"

#include <chrono>
#include <condition_variable>
#include <thread>

#include "backend/RenderTarget.h"

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {
   
   class CompositorScheduler {
       Compositor *compositor;
   public:
        bool shutdown;

        void processCommand(CompositionRenderCommand & command);
        std::mutex mutex;
        std::thread t;

       explicit CompositorScheduler(Compositor *compositor);
       ~CompositorScheduler();
   };

   enum class RenderCommandStatus : int {
       Ok,
       Failed,
       Delayed
   };

   struct CompareCommands {
        auto operator()(SharedHandle<Composition::CompositorCommand> & lhs,SharedHandle<Composition::CompositorCommand> & rhs){
            if(lhs->type == CompositorCommand::View){
                return true;
            }
            else if(rhs->type == CompositorCommand::View){
                return false;
            }
            else if(lhs->type == CompositorCommand::HoldRender){
                return true;
            }
            else if(rhs->type == CompositorCommand::HoldRender){
                return false;
            }
            else if(lhs->type == CompositorCommand::ResumeRender){
                return true;
            }
            else if(rhs->type == CompositorCommand::ResumeRender){
                return false;
            }
            else if(lhs->type == CompositorCommand::Render && rhs->type == CompositorCommand::Render) {
                auto _lhs = std::dynamic_pointer_cast<CompositionRenderCommand>(lhs);
                auto _rhs = std::dynamic_pointer_cast<CompositionRenderCommand>(rhs);
                bool cond1, cond2;
                cond1 = _lhs->thresholdParams.timeStamp < _rhs->thresholdParams.timeStamp;
                cond2 = _lhs->thresholdParams.hasThreshold ? (_rhs->thresholdParams.hasThreshold ? (
                        _lhs->thresholdParams.threshold < _rhs->thresholdParams.threshold) : true)
                                                         : _rhs->thresholdParams.hasThreshold
                                                           ? (_lhs->thresholdParams.hasThreshold ? (
                                        _rhs->thresholdParams.threshold < _lhs->thresholdParams.threshold) : false) : true;
                // printf("Sort Cond:%i\n",((cond1 == true) && (cond2 == true)));
                return cond1 && cond2;
            }
        };
    };

   
    /**
     OmegaWTK's Composition Engine Frontend Interface
     */
    class OMEGAWTK_EXPORT Compositor {

        OmegaCommon::Vector<LayerTree *> targetLayerTrees;

        RenderTargetStore renderTargetStore;

        std::mutex * queueMutex;

        bool queueIsReady;

        std::condition_variable queueCondition;
        Core::PriorityQueueHeap<SharedHandle<CompositorCommand>,CompareCommands> commandQueue;

        friend class CompositorClient;
        friend class CompositorScheduler;

        CompositorScheduler scheduler;

        CompositionRenderCommand *currentCommand = nullptr;

        bool allowUpdates = false;

        friend class Layer;
        friend class LayerTree;
        friend class WindowLayer;

        std::future<RenderCommandStatus> executeCurrentRenderCommand();

    public:
        void scheduleCommand(SharedHandle<CompositorCommand> & command);
        
        
        Compositor();
        ~Compositor();
    };

    struct BackendExecutionContext {

    };
}

#endif
