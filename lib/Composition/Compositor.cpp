#include "omegaWTK/Composition/Compositor.h"
#include "omegaWTK/Composition/Backend.h"
#include "omegaWTK/Composition/Layer.h"

namespace OmegaWTK::Composition {
Compositor::Compositor():backend(make_backend()){};

void Compositor::prepareDraw(Layer *root){
    backend->setCurrentJob(root);
    backend->doWork();
    auto layer_it = root->children.begin();
    while(layer_it != root->children.end()){
        backend->setCurrentJob(*layer_it);
        backend->doWork();
        ++layer_it;
    };
    backend->setCurrentJob(nullptr);
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
