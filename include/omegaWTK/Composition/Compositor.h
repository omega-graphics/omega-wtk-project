#include "omegaWTK/Core/Core.h"
#include "Layer.h"

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {

    class Backend;
    /**
     OmegaWTK's Composition Engine Frontend Interface
     */
    class OMEGAWTK_EXPORT Compositor {
        Core::UniquePtr<Backend> backend;
        bool allowUpdates = false;
        friend class Layer;
        friend class LayerTree;
        friend class WindowLayer;
        void __drawChildLimbs(LayerTree::Limb *limb,LayerTree *layerTree);
        /// Updates a Requested layer
        OMEGAWTK_DEPRECATED void updateRequestedLayer(Layer *layer);
        void updateRequestedLayerTreeLimb(LayerTree::Limb *limb);
        void updateWindowLayer(WindowLayer *layer);
        void layoutLayerTreeLimb(LayerTree::Limb *limb);
        public:
        /// Initial Draw for Widget
        void prepareDraw(LayerTree *treeToDraw);
        /// Initial Draw on WindowLayer
        void prepareDrawWindow(WindowLayer *window);
        /// Cleanup (Called when app closes)
        void prepareCleanup();
        Compositor();
        ~Compositor();
    };

}

#endif
