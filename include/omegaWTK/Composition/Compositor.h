#include "Layer.h"

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {


    class Compositor {
        Layer *rootLayer;
        public:
        void prepareDraw();
        void prepareCleanup();
        void prepareUpdate();
        Compositor(Layer *layer);
    };

}

#endif
