#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK::Composition {
    Layer::Layer(const Core::Rect & rect):surface_rect(rect){};

    Canvas * Layer::getParentCanvas(){
        return parentCanvas;
    };

}