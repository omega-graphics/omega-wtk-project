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

Backend::Backend():currentLimb(nullptr){
//    occupied_condition.notify_one();
};

Backend::~Backend(){
    
};

void Backend::setCurrentJob(LayerTree::Limb *limb){
    currentLimb = limb;
};

Core::UniquePtr<Backend> make_backend(){
    return std::make_unique<BackendImpl>();
};

};
