#include "omegaWTK/Composition/Backend.h"
#include "omegaWTK/Composition/Layer.h"

#ifdef TARGET_WIN32
#include "win/DirectXBackend.h"
#endif

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
#ifdef TARGET_WIN32
    return make_directx_backend();
#endif
#ifdef TARGET_MACOS
    return make_quartz_backend();
#endif
};

};
