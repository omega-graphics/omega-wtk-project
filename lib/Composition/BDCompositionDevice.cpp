#include "BDCompositionDevice.h"
#include "BDCompositionRenderTarget.h"

#ifdef TARGET_WIN32
#include "dx/DXBDCompositionDevice.h"
#endif

#ifdef TARGET_MACOS
#include "mtl/MTLBackend.h"
#endif

namespace OmegaWTK::Composition {

Core::SharedPtr<BDCompositionDevice> BDCompositionDevice::Create(){
    #ifdef TARGET_WIN32
    return DXBDCompositionDevice::Create();
    #endif
    #ifdef TARGET_MACOS
    return CreateMTLBDCompositonDevice();
    #endif
};
}