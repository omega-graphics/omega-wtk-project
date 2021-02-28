#include "../BDCompositionImage.h"
#include "MTLBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONIMAGE_H

namespace OmegaWTK::Composition {

template<class _Ty>
class MTLBDCompositionRenderTarget;

class MTLBDCompositionImage : public BDCompositionImage {
    MTLTextureDescriptor *desc;
    Core::Rect n_rect;
    Core::FRect rect;
    friend class MTLBDCompositionRenderTarget<BDCompositionLayerRenderTarget>;
    friend class MTLBDCompositionRenderTarget<BDCompositionImageRenderTarget>;
    friend class MTLBDCompositionDevice;
public:
    id<MTLTexture> img;
    /**
     Create a Composition from an existing Core::BitmapImage and a id<MTLTexture>
     */
    MTLBDCompositionImage(Core::SharedPtr<Core::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img);
    /**
     Create a Compositon Image from a id<MTLTexture>
    */
    MTLBDCompositionImage(Core::Rect & rect,id<MTLTexture> img);
    static Core::SharedPtr<BDCompositionImage> Create(Core::SharedPtr<Core::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img);
    static Core::SharedPtr<BDCompositionImage> Create(Core::Rect & rect,id<MTLTexture> img);
};

MTLPixelFormat computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb);

};

#endif
