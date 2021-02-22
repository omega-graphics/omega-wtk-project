#include "omegaWTK/Composition/Backend.h"
#include "omegaWTK/Composition/Layer.h"
#include "BackendPriv.h"

// #ifdef TARGET_WIN32
// #include "win/DirectXBackend.h"
// #endif

//#ifdef TARGET_MACOS
//#include "macos/QuartzBackend.h"
//#endif

namespace OmegaWTK::Composition {

Backend::Backend():currentLayer(nullptr){
    
};

Backend::~Backend(){
    
};

void Backend::setCurrentJob(Layer *layer){
    currentLayer = layer;
};

Core::UniquePtr<Backend> make_backend(){
    return std::make_unique<BackendImpl>(BackendImpl());
};

};
