#include "../BDCompositionImage.h"
#include "MTLBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONIMAGE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONIMAGE_H

@class CIImage;

namespace OmegaWTK::Composition {

template<class _Ty>
class MTLBDCompositionRenderTarget;
class MTLBDCALayerTree;

class MTLBDCompositionImage : public BDCompositionImage {
    MTLTextureDescriptor *desc;
    Core::Rect n_rect;
    Core::FRect rect;
    LayerEffect *dropShadow = nullptr;
    Core::Vector<LayerEffect *> imageEffects;
    MTLBDCompositionDeviceContext *deviceContext;
    friend class MTLBDCompositionRenderTarget<BDCompositionViewRenderTarget>;
    friend class MTLBDCompositionRenderTarget<BDCompositionImageRenderTarget>;
    friend class MTLBDCompositionDeviceContext;
    friend class MTLBDCALayerTree;
public:
    id<MTLTexture> img;
    /**
     Create a Composition from an existing Core::BitmapImage and a id<MTLTexture>
     */
    MTLBDCompositionImage(MTLBDCompositionDeviceContext *deviceContext,Core::SharedPtr<Media::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img);
    /**
     Create a Compositon Image from a id<MTLTexture>
    */
    MTLBDCompositionImage(MTLBDCompositionDeviceContext *deviceContext,Core::Rect & rect,id<MTLTexture> img,MTLTextureDescriptor *desc);
    static Core::SharedPtr<BDCompositionImage> Create(MTLBDCompositionDeviceContext *deviceContext,Core::SharedPtr<Media::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img);
    static Core::SharedPtr<BDCompositionImage> Create(MTLBDCompositionDeviceContext *deviceContext,Core::Rect & rect,id<MTLTexture> img,MTLTextureDescriptor *desc);
    void applyEffect(LayerEffect *effect);
    /**
     Will only commit Image Compatible Effects
     */
    void commitEffects();
};

MTLPixelFormat computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb);

};

#endif
