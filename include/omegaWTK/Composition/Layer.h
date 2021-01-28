#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_LAYER_H
#define OMEGAWTK_COMPOSITION_LAYER_H

namespace OmegaWTK {
    namespace Composition {

        class Canvas;
        /// A surface upon which visuals can be drawn.
        /// `INTERFACE`
        class Layer {
            Canvas *parentCanvas;
            friend class Canvas;
            public:
            Canvas *getParentCanvas();
            Layer();
        };
    };
};

#endif