
#include "BDCompositionRenderTarget.h"
#include "BDCompositionDevice.h"

namespace OmegaWTK::Composition {

    Core::SharedPtr<BDCompositionImage> & BDCompositionRenderTarget::getImage(unsigned visual_id){
        return images[visual_id];
    };

}
