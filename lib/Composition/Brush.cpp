#include "omegaWTK/Composition/Brush.h"

namespace OmegaWTK::Composition {

Brush::Brush(const Color & color):color(color),isColor(true),isGradient(false){
    
};

Brush::Brush(const Gradient & gradient):color(0,0),isColor(false),gradient(gradient),isGradient(true){
    
};

Core::SharedPtr<Brush> ColorBrush(const Color & color){
    return std::make_shared<Brush>(color);
};

Core::SharedPtr<Brush> GradientBrush(const Gradient & gradient){
    return std::make_shared<Brush>(gradient);
};

}
