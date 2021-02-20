#include "BDCompositionDevice.h"
#include "BDCompositionRenderTarget.h"

#ifdef TARGET_WIN32
#include "dx/DXBDCompositionDevice.h"
#endif

namespace OmegaWTK::Composition {

Core::SharedPtr<BDCompositionDevice> BDCompositionDevice::Create(){
    #ifdef TARGET_WIN32
    return DXBDCompositionDevice::Create();
    #endif
};
}
