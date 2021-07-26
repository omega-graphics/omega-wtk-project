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



   struct CompositionRenderCommandThreasholdParams {
       bool hasThreshold;
       std::chrono::time_point<std::chrono::high_resolution_clock> timeStamp;
       std::chrono::time_point<std::chrono::high_resolution_clock> threshold;
   };

   struct CompositionRenderCommand {
       typedef enum {
           Low,
           High,
       } Priority;
       Priority priority;
       CompositionRenderCommandThreasholdParams thresholdParams;
       CompositionRenderTarget *renderTarget;
       VisualCommand **_visuals;
       unsigned visual_count;
   };
   
   class CompositorScheduler {
       Compositor *compositor;
   public:
        bool shutdown;

        void processCommand(CompositionRenderCommand & command);
        std::mutex mutex;
        std::thread t;

       CompositorScheduler(Compositor *compositor);
       ~CompositorScheduler();
   };

   enum class RenderCommandStatus : int {
       Ok,
       Failed,
       Delayed
   };

   struct CompareRenderCommands {
        auto operator()(Composition::CompositionRenderCommand & lhs,Composition::CompositionRenderCommand & rhs){
            bool cond1,cond2;
            cond1 = lhs.thresholdParams.timeStamp < rhs.thresholdParams.timeStamp;
            cond2 = lhs.thresholdParams.hasThreshold? (rhs.thresholdParams.hasThreshold? (lhs.thresholdParams.threshold < rhs.thresholdParams.threshold) : true) : rhs.thresholdParams.hasThreshold? (lhs.thresholdParams.hasThreshold? (rhs.thresholdParams.threshold < lhs.thresholdParams.threshold) : false)  : true;
            // printf("Sort Cond:%i\n",((cond1 == true) && (cond2 == true)));
            return (cond1 == true) && (cond2 == true);
        };
    };

   
    /**
     OmegaWTK's Composition Engine Frontend Interface
     */
    class OMEGAWTK_EXPORT Compositor : public LayerTreeObserver {

        OmegaCommon::Vector<LayerTree *> targetLayerTrees;

        RenderTargetStore renderTargetStore;

        std::mutex * queueMutex;

        bool queueIsReady;

        std::condition_variable queueCondition;
        Core::PriorityQueueHeap<CompositionRenderCommand,CompareRenderCommands> commandQueue;

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
        void layerHasEnabled(Layer *layer) override;
        void layerHasResized(Layer *layer) override;
        void layerHasDisabled(Layer *layer) override;
        void scheduleCommand(CompositionRenderCommand * command);
        
        
        Compositor();
        ~Compositor();
    };

    struct BackendExecutionContext {

    };
}

#endif
