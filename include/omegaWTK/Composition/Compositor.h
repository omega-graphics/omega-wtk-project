#include "Layer.h"

#ifndef OMEGAWTK_COMPOSTION_COMPOSITOR_H
#define OMEGAWTK_COMPOSTION_COMPOSITOR_H

namespace OmegaWTK::Composition {


    class Compositor {
        Layer *rootLayer;
        public:
        void draw();
        void erase();
    };

}

#endif