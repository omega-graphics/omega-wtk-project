
#include "BDCompositionRenderTarget.h"
#include "BDCompositionDevice.h"

namespace OmegaWTK::Composition {

Core::SharedPtr<BDCompositionRenderTarget> BDCompositionRenderTarget::Create(BDCompositionDevice * device,Layer *layer){
    
};

void BDCompositionRenderTarget::addBrush(Core::SharedPtr<Brush> & brush){
    brushes.push_back(brush);
};

void BDCompositionRenderTarget::removeBrush(Core::SharedPtr<Brush> & brush){
    auto it = brushes.begin();
    while(it != brushes.end()){
        if(*it == brush){
            brushes.erase(it);
            break;
        };
        ++it;
    };
};

}
