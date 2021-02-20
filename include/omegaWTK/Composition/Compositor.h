#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {

    class Layer;

    class Backend;
    class OMEGAWTK_EXPORT Compositor {
        Core::UniquePtr<Backend> backend;
        bool allowUpdates = false;
        friend class Layer;
        void updateRequestLayer(Layer *layer);
        public:
        /// Initial Draw
        void prepareDraw(Layer *root);
        /// Cleanup (Called when app closes, or widget hides)
        void prepareCleanup();
        Compositor();
        ~Compositor();
    };

}

#endif
