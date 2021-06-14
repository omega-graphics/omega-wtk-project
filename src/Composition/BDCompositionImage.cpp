#include "BDCompositionImage.h"

namespace OmegaWTK::Composition {
    BDCompositionImage::BDCompositionImage(Core::SharedPtr<Media::BitmapImage> & img):bitmapImage(img){

    };
    BDCompositionImage::BDCompositionImage():bitmapImage(nullptr){};
};
