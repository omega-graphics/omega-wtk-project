#include "../BDCompositionImage.h"
#include "MTLBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONIMAGE_H

namespace OmegaWTK::Composition {

class MTLBDCompositionImage : public BDCompositionImage {
    MTLTextureDescriptor *desc;
    Core::Rect & rect;
    friend class MTLBDCompositionRenderTarget;
public:
    id<MTLTexture> img;
    MTLBDCompositionImage(Core::SharedPtr<Core::BitmapImage> & _img,Core::Rect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img);
    static Core::SharedPtr<BDCompositionImage> Create(Core::SharedPtr<Core::BitmapImage> & _img,Core::Rect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img);
};

MTLPixelFormat computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb);

};

#endif
