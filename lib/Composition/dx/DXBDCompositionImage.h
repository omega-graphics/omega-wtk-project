#include "../BDCompositionImage.h"
#include "DXBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONIMAGE_H

namespace OmegaWTK::Composition {
    template<class _Ty>
    class IDXBDCompositionRenderTarget;
    typedef IDXBDCompositionRenderTarget<None> DXBDCompositionRenderTarget;

    class DXBDCompositionViewRenderTarget;
    class DXBDCompositionImageRenderTarget;
    class DXBDCompositionImage : public BDCompositionImage {
        DXBDCompositionViewRenderTarget *viewRenderTarget;
        DXBDCompositionImageRenderTarget *imageRenderTarget;
        bool createdByImageRenderTarget;
        bool createdByLayerRenderTarget;
        bool createdFromBitmap;
        DXBDCompositionRenderTarget * getTarget();
        Core::Rect & rect;
        friend class DXBDCompositionViewRenderTarget;
        friend class DXBDCompositionImageRenderTarget;
        friend class DXBDCompositionDeviceContext;
    public:
        Core::UniqueComPtr<ID2D1Image> native_image;
        void redoImage();
        // void applyEffect(VisualEffect &effect);
        /**
        Create a DXBDCompositionImage from a Core::BitmapImage and a ID2D1Image. (Created by a Layer Render Target)
        */
        DXBDCompositionImage(Core::SharedPtr<Media::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionViewRenderTarget *renderTarget,ID2D1Image *img);
        /**
        Create a DXBDCompositionImage from a Core::BitmapImage and a ID2D1Image. (Created by an Image Render Target)
        */
        DXBDCompositionImage(Core::SharedPtr<Media::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionImageRenderTarget *renderTarget,ID2D1Image *img);
        /**
        Create a DXBDCompositionImage from a ID2D1Image. (Created by a Layer Render Target)
        */
        DXBDCompositionImage(Core::Rect & rect,DXBDCompositionViewRenderTarget *renderTarget,ID2D1Image *img);
        /**
        Create a DXBDCompositionImage from a ID2D1Image. (Created by an Image Render Target)
        */
        DXBDCompositionImage(Core::Rect & rect,DXBDCompositionImageRenderTarget *renderTarget,ID2D1Image *img);
        static Core::SharedPtr<BDCompositionImage> Create(Core::SharedPtr<Media::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionViewRenderTarget *renderTarget,ID2D1Image *img);
        static Core::SharedPtr<BDCompositionImage> Create(Core::SharedPtr<Media::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionImageRenderTarget *renderTarget,ID2D1Image *img);
        static Core::SharedPtr<BDCompositionImage> Create(Core::Rect & rect,DXBDCompositionViewRenderTarget *renderTarget,ID2D1Image *img);
        static Core::SharedPtr<BDCompositionImage> Create(Core::Rect & rect,DXBDCompositionImageRenderTarget *renderTarget,ID2D1Image *img);
    };

    DXGI_FORMAT computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb);
};

#endif