#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_BRUSH_H
#define OMEGAWTK_COMPOSITION_BRUSH_H

namespace OmegaWTK::Composition {

    /// @brief Represents a generic RGBA Color with a channel size from 8 to 32 bits.
    struct OMEGAWTK_EXPORT Color {
        float r,g,b,a;
    public:
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
        /// @brief 8 Bit Standard Colors (32bit color)
        enum Eight : uint32_t {
            Black8  = 0x000000,
            White8  = 0xFFFFFF,
            Red8    = 0xFF0000,
            Green8  = 0x00FF00,
            Blue8   = 0x0000FF,
            Yellow8 = 0xFFFF00,
            Orange8 = 0xFF8000,
            Purple8 = 0xFF00FF
        };
        /// @brief 16 Bit Standard Colors (64bit color)
        enum Sixteen : uint64_t {
            Black16  = 0x000000000000,
            White16  = 0xFFFFFFFFFFFF,
            Red16    = 0xFFFF00000000,
            Green16  = 0x0000FFFF0000,
            Blue16   = 0x00000000FFFF,
            Yellow16 = 0xFFFFFFFF0000,
            Orange16 = 0xFFFF88000000,
            Purple16 = 0xFFFF0000FFFF
        };
        static Color create8Bit(std::uint8_t r,std::uint8_t g,std::uint8_t b,std::uint8_t a);
        static Color create8Bit(std::uint32_t rgb,std::uint8_t alpha = 0xFF);
        static Color create16Bit(std::uint16_t r,std::uint16_t g,std::uint16_t b,std::uint16_t a);
        static Color create16Bit(std::uint64_t rgb,std::uint16_t alpha = 0xFFFF);
        static Color create32it(std::uint32_t r,std::uint32_t g,std::uint32_t b,std::uint32_t a);
        static const Color Transparent;
        ~Color() = default;
    };


    struct OMEGAWTK_EXPORT Gradient {
        enum class Type : OPT_PARAM {
            Linear,
            Radial
        } type;
        float arg;
        struct GradientStop {
            float pos;
            Color color;
        };
        OmegaCommon::Vector<GradientStop> stops;
        static GradientStop Stop(float pos,Color color);
        static Gradient Linear(std::initializer_list<GradientStop> stops,float angle);
        static Gradient Radial(std::initializer_list<GradientStop> stops,float radius);
        ~Gradient() = default;
    };

    /// @brief Represents a Brush used for filling in color or texture of vector graphics.
    struct OMEGAWTK_EXPORT  Brush {
        OMEGACOMMON_CLASS("OmegaWTK.Composition.Brush")
        bool isColor;
        bool isGradient;
        union {
            Color color;
            Gradient gradient;
        };
        /**
         @brief Constructs a Color Brush!
         */
        Brush(const Color & color);
        /**
         @brief Constructs a Gradient Brush!
         */
        Brush(const Gradient & gradient);

        ~Brush();
    };

OMEGAWTK_EXPORT Core::SharedPtr<Brush> ColorBrush(const Color &color);
OMEGAWTK_EXPORT Core::SharedPtr<Brush> GradientBrush();

};

#endif
