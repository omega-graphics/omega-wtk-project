#include "Layer.h"

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {

    class Backend;
    class OMEGAWTK_EXPORT Compositor {
        Layer *rootLayer;
        Backend *backend;
        public:
        void prepareDraw();
        void prepareCleanup();
        void prepareUpdate();
        Compositor(Layer *layer);
    };

}

#endif
