#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/ImgCodec.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONIMAGE_H

namespace OmegaWTK::Composition {
    /// A Fully Pre-Processed Image Ready to be Drawn/Used!
    class BDCompositionImage {
    protected:
        Core::SharedPtr<Core::BitmapImage> bitmapImage;
    public:
        BDCompositionImage(Core::SharedPtr<Core::BitmapImage> & img);
        #ifdef TARGET_WIN32
        virtual void redoImage() = 0;
        #endif
        // static Core::SharedPtr<BDCompositionImage> Create(Core::SharedPtr<Core::BitmapImage> & img);
    };
};

#endif