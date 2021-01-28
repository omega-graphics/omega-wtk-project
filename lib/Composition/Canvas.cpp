#include "omegaWTK/Composition/Canvas.h"
#include "omegaWTK/Composition/Layer.h"

namespace OmegaWTK::Composition {

     Canvas::Canvas(unsigned width,unsigned height,const CanvasDrawOpts & opts):w(width),h(height){};

    void Canvas::addLayer(Layer * layer){
        layers.push(layer);
        current_layer = layers.last();
    };

    void Canvas::insertLayer(Layer * layer,unsigned idx){
        layers.insert(layer,idx);
    };

    void Canvas::setCurrentLayer(unsigned idx){
        current_layer = layers[idx];
    };

}