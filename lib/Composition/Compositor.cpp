#include "omegaWTK/Composition/Compositor.h"
#include "omegaWTK/Composition/Backend.h"

namespace OmegaWTK::Composition {
Compositor::Compositor(Layer *layer):rootLayer(layer),backend(make_backend()){};

void Compositor::prepareDraw(){
    
};
void Compositor::prepareCleanup(){
    
};

void Compositor::prepareUpdate(){
    
};

};
