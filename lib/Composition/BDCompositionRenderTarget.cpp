
#include "BDCompositionRenderTarget.h"
#include "BDCompositionDevice.h"

namespace OmegaWTK::Composition {

    Core::SharedPtr<BDCompositionImage> & BDCompositionRenderTarget::getImage(unsigned visual_id){
        return images[visual_id];
    };

// void BDCompositionRenderTarget::addBrush(Core::SharedPtr<Brush> & brush){
//     brushes.push_back(brush);
// };

// void BDCompositionRenderTarget::removeBrush(Core::SharedPtr<Brush> & brush){
//     auto it = brushes.begin();
//     while(it != brushes.end()){
//         if(*it == brush){
//             brushes.erase(it);
//             break;
//         };
//         ++it;
//     };
// };

//  bool BDCompositionRenderTarget::hasBrush(Core::SharedPtr<Brush> & brush){
//         for(auto & b : brushes){
//             if(b == brush){
//                 return true;
//             };
//         };
//         return false;
//     };

}
