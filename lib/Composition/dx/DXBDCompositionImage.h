#include "../BDCompositionImage.h"
#include "DXBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONIMAGE_H

namespace OmegaWTK::Composition {
    class DXBDCompositionImage : public BDCompositionImage {
        DXBDCompositionRenderTarget *renderTarget;
        Core::Rect & rect;
        friend class DXBDCompositionRenderTarget;
    public:
        Core::UniqueComPtr<ID2D1Image> native_image;
        void redoImage();
        DXBDCompositionImage(Core::SharedPtr<Core::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionRenderTarget *renderTarget,ID2D1Image *img);
        static Core::SharedPtr<BDCompositionImage> Create(Core::SharedPtr<Core::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionRenderTarget *renderTarget,ID2D1Image *img);
    };

    DXGI_FORMAT computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb);
};

#endif