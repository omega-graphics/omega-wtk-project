#include "omegaWTK/Composition/Brush.h"

namespace OmegaWTK::Composition {

Brush::Brush(const Color & color):isColor(true),color(color),isGradient(false){
    
};

Brush::Brush(const Gradient & gradient):isColor(false),color(0,0),isGradient(true),gradient(gradient){
    
};

Core::SharedPtr<Brush> ColorBrush(const Color & color){
    return std::make_shared<Brush>(color);
};

Core::SharedPtr<Brush> GradientBrush(const Gradient & gradient){
    return std::make_shared<Brush>(gradient);
};

}
