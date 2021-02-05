#include "omegaWTK/Composition/Compositor.h"
#include "omegaWTK/Composition/Backend.h"

namespace OmegaWTK::Composition {
Compositor::Compositor():backend(make_backend()){};

void Compositor::prepareDraw(Layer *root){
    backend->setCurrentJob(root);
    backend->doWork();
    allowUpdates = true;
};
void Compositor::prepareCleanup(){
    allowUpdates = false;
};

void Compositor::updateRequestLayer(Layer *layer){
    if(allowUpdates){
        backend->setCurrentJob(layer);
        backend->doUpdate();
    };
};

Compositor::~Compositor(){
    prepareCleanup();
};


};
