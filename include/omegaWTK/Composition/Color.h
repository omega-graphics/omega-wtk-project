#include "omegaWTK/Core/Core.h"
#include <cstdint>

#ifndef OMEGAWTK_COMPOSITION_COLOR_H
#define OMEGAWTK_COMPOSITION_COLOR_H


namespace OmegaWTK {
    namespace Composition {
        /// Standard RBGA Color
        struct OMEGAWTK_EXPORT Color {
            unsigned r,g,b,a;
        public:
            typedef enum : std::uint32_t {
                Black  = 0x000000,
                White  = 0xFFFFFF,
                Red    = 0xFF0000,
                Green  = 0x00FF00,
                Blue   = 0x0000FF,
                Yellow = 0xFFFF00,
                Orange = 0xFF8000,
                Purple = 0xFF00FF
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
            /**
                Constructs a color!
                @param red red amount
                @param green green amount
                @param blue blue amount
                @param alpha alpha value
            */
            Color(unsigned red,unsigned green,unsigned blue,unsigned alpha);
            /**
             Constructs a Color using a 32bit uint (In hexadecimal form)!
             @param hex_color Hexadecimal color

            hex_color must be in the following form
            --> 0xRRGGBB
            */ 
            Color(std::uint32_t hex_color,std::uint8_t alpha = 0xFF);
            static const Color Transparent;
        };

        struct OMEGAWTK_EXPORT Gradient {
             typedef enum : OPT_PARAM {
                Linear,
                Radial
            } GradientType;
            GradientType type;
            struct GradientStop {
                float pos;
                Color color;
            };
            OmegaCommon::Vector<GradientStop> stops;
            static GradientStop CreateStop(float pos,Color color);
            static Gradient CreateLinear(std::initializer_list<GradientStop> stops);
            static Gradient CreateRadial(std::initializer_list<GradientStop> stops);
        };
    };
};



#endif
