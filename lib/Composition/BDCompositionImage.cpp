#include "BDCompositionImage.h"

namespace OmegaWTK::Composition {
    BDCompositionImage::BDCompositionImage(Core::SharedPtr<Core::BitmapImage> & img):bitmapImage(img){

    };
    BDCompositionImage::BDCompositionImage():bitmapImage(nullptr){};
};
