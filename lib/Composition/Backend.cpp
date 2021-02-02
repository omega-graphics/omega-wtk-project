#include "omegaWTK/Composition/Backend.h"
#include "omegaWTK/Composition/Layer.h"

#ifdef TARGET_MACOS
#include "macos/QuartzBackend.h"
#endif

namespace OmegaWTK::Composition {

Backend::Backend():currentLayer(nullptr){
    
};

Backend::setCurrentJob(Layer *layer){
    currentLayer = layer;
};

};
