#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_COLOR_H
#define OMEGAWTK_COMPOSITION_COLOR_H


namespace OmegaWTK {
    namespace Composition {
        /// Standard RBGA Color
        struct OMEGAWTK_EXPORT Color {
            unsigned r,g,b,a;
            /**
                Constructs a color!
                @param red red amount
                @param green green amount
                @param blue blue amount
                @param alpha alpha value
            */
        public:
            typedef enum : OPT_PARAM {
                Red,
                Green,
                Blue,
                Yellow,
                Orange
            } STDColor;
            bool compare(const Color &other);
            bool operator!=(Color && other){
                return !compare(other);
            };
            bool operator!=(const Color &other){
                return !compare(other);
            };
            bool operator==(Color && other){
                return compare(other);
            };
            bool operator==(const Color &other){
                return compare(other);
            };
            Color(unsigned red,unsigned green,unsigned blue,unsigned alpha);
            Color(STDColor ty);
        };
    };
};



#endif
