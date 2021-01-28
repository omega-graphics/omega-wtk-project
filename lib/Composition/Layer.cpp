#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK::Composition {
    Layer::Layer(){};

    Canvas * Layer::getParentCanvas(){
        return parentCanvas;
    };

}