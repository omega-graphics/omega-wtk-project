#include "Color.h"

#ifndef OMEGAWTK_COMPOSITION_BRUSH_H
#define OMEGAWTK_COMPOSITION_BRUSH_H

namespace OmegaWTK::Composition {

    struct OMEGAWTK_EXPORT  Brush {
        bool isColor;
        Color color;
        bool isGradient;
        Gradient gradient;
        /**
         Constructs a Color Brush!
         */
        Brush(const Color & color);
        /**
         Constructs a Gradient Brush!
         */
        Brush(const Gradient & gradient);
    };

Core::SharedPtr<Brush> ColorBrush(const Color &color);
Core::SharedPtr<Brush> GradientBrush();

};

#endif
