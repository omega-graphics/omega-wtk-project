#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Media/ImgCodec.h"
#include "omegaWTK/Composition/Visual.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONIMAGE_H

namespace OmegaWTK::Composition {
    /// A Fully Pre-Processed Image Ready to be Drawn/Used!
    class BDCompositionImage {
    protected:
        Core::SharedPtr<Media::BitmapImage> bitmapImage;
    public:
        /**
         Create a Blank Composition Image
         */
        BDCompositionImage();
        /**
         Create a Composition Image from a Core::BitmapImage
         */
        BDCompositionImage(Core::SharedPtr<Media::BitmapImage> & img);
        #ifdef TARGET_WIN32
        virtual void redoImage() = 0;
        #endif
        #if !defined(TARGET_WIN32)
        virtual void applyEffect(LayerEffect *effect) = 0;
        virtual void commitEffects() = 0;
        #endif
    
    };
};

#endif
