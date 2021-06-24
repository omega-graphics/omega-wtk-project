#include "omegaWTK/Core/Core.h"
#include "Layer.h"
#include "CompositorClient.h"
#include <chrono>
#include <thread>

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {

    class Backend;


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
       VisualCommand *_visuals;
       unsigned visual_count;
   };
   
   class Scheduler {
   public:
        bool shutdown;
        std::mutex mutex;
       std::thread * t;
       void run();
       Scheduler();
       ~Scheduler();
   };

   
    /**
     OmegaWTK's Composition Engine Frontend Interface
     */
    class OMEGAWTK_EXPORT Compositor {
        friend class CompositorClient;
        Core::UniquePtr<Backend> backend;

        Core::UniquePtr<Scheduler> scheduler;

        bool allowUpdates = false;

        friend class Layer;
        friend class LayerTree;
        friend class WindowLayer;

        // void __drawChildLimbs(LayerTree::Limb *limb,LayerTree *layerTree);
        // void __updateChildLimbs(LayerTree::Limb *limb,LayerTree *layerTree);
        // /// Updates a Requested layer

        // OMEGAWTK_DEPRECATED void updateRequestedLayer(Layer *layer);
        // void updateRequestedLayerTreeLimb(LayerTree::Limb *limb);
        // void updateWindowLayer(WindowLayer *layer);
        // void layoutLayerTreeLimb(LayerTree::Limb *limb);
    public:
        void scheduleCommand(UniqueHandle<CompositionRenderCommand> command);
        
        // void updateLayerTree(LayerTree * tree);
        // void prepareDraw(LayerTree *tree);
        // void prepareDrawWindow(WindowLayer *layer);
        // void prepareCleanup();
        
        Compositor();
        ~Compositor();
    };
}

#endif
