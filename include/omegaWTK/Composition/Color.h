#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_COLOR_H
#define OMEGAWTK_COMPOSITION_COLOR_H


namespace OmegaWTK {
    namespace Composition {
        /// Standard RBGA Color
        class Color {
            unsigned r,g,b,a;
            /**
                Constructs a color!
                @param red red amount
                @param green green amount
                @param blue blue amount
                @param alpha alpha value
            */
            Color(unsigned red,unsigned green,unsigned blue,unsigned alpha);
        };
    };
};



#endif