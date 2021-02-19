#include "BDCompositionDevice.h"
#include "BDCompositionRenderTarget.h"

namespace OmegaWTK::Composition {

Core::SharedPtr<BDCompositionDevice> BDCompositionDevice::Create(){
    
};

Core::SharedPtr<BDCompositionRenderTarget> BDCompositionDevice::makeTarget(Layer *layer){
    auto store = BDCompositionRenderTarget::Create(this);
    targets.insert(std::make_pair(layer,store));
    
};
}
