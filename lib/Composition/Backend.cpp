#include "omegaWTK/Composition/Backend.h"
#include "omegaWTK/Composition/Layer.h"

#ifdef TARGET_MACOS
#include "macos/QuartzBackend.h"
#endif

namespace OmegaWTK::Composition {

Backend::Backend():currentLayer(nullptr){
    
};

Backend::~Backend(){
    
};

void Backend::setCurrentJob(Layer *layer){
    currentLayer = layer;
};

Backend * make_backend(){
#ifdef TARGET_MACOS
    return make_quartz_backend();
#endif
};

};
