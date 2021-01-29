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
            protected:
            Core::Rect surface_rect;
            public:
            Canvas *getParentCanvas();
            Layer(const Core::Rect & rect);
        };
    };
};

#endif