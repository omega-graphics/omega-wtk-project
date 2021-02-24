#include "../BDCompositionRenderTarget.h"
#include "MTLBDCompositionDevice.h"
#include "MTLBDTriangulator.h"

#include "NativePrivate/macos/CocoaItem.h"

#import <QuartzCore/QuartzCore.h>

#include <functional>

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONRENDERTARGET_H



namespace OmegaWTK::Composition {

    class MTLBDCompositionRenderTarget : public BDCompositionRenderTarget {
        MTLBDCompositionDevice *device;
        Native::Cocoa::CocoaItem *native_item;
        Core::UniquePtr<MTLBDTriangulator> triangulator;
        CAMetalLayer *metalLayer;
        Core::Vector<id<MTLCommandBuffer>> commandBuffers;
        Core::Vector<id<MTLBuffer>> vertexBuffers;
        Color clearColor;
        Core::Map<unsigned,id<MTLTexture>> textures;
        
        struct RenderPipeline {
            bool multiSampled = false;
            unsigned sampleCount = 0;
            id<MTLRenderPipelineState> pipelineState;
            std::function<void(id<MTLRenderCommandEncoder> encoder,unsigned v_id)> setupCallback;
        };
        Core::Queue<RenderPipeline> renderPasses;
    public:
        MTLBDCompositionRenderTarget(MTLBDCompositionDevice *device,Native::Cocoa::CocoaItem *item);
        static Core::SharedPtr<BDCompositionRenderTarget> Create(MTLBDCompositionDevice *device,Native::Cocoa::CocoaItem *item);
        void clear(Color &clear_color);
        void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned width);
        void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned width);
        void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
        void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush);
        void drawText(Core::SharedPtr<BDCompositionFont> &font, Core::String &string, Core::Rect &textRect, Core::SharedPtr<Brush> &brush);
        Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> &img, Core::Rect &newSize, unsigned v_id);
        void drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos);
        void commit();
    };
};

#endif
