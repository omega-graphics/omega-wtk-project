#include "../BDCompositionDevice.h"
#include "../BDCompositionRenderTarget.h"

#import <Metal/Metal.h>

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {

class MTLBDCompositionViewRenderTarget;

class MTLBDCompositionDevice : public BDCompositionDevice {
public:
    id<MTLDevice> metal_device;
    id<MTLLibrary> metal_default_library;
    /**
     Pipeline States
     */
    id<MTLRenderPipelineState> solidColorPrimitive;
    id<MTLRenderPipelineState> texture2DPrimitive;
    id<MTLRenderPipelineState> texture2DPrimitiveWithBkgrd;
    /**
     Metal Library Functions
     */
    id<MTLFunction> solidColorVertex;
    id<MTLFunction> solidColorFragment;
    id<MTLFunction> texture2DVertex;
    id<MTLFunction> texture2DFragment;
    id<MTLFunction> texture2DFragmentWithBkgrd;
private:
//    Core::Vector<id<MTLCommandQueue>> metal_command_queues;
    id<MTLCommandQueue> metal_command_queue;
    unsigned bufferCount;
    inline id<MTLRenderPipelineState> setupPipelineState(id<MTLFunction> vertexFunc,id<MTLFunction> fragmentFunc,MTLPixelFormat pixelFormat);
public: 
    id<MTLRenderPipelineState> makeMultiSampledPipelineState(bool textured,unsigned sampleCount,NSString *label);
    MTLBDCompositionDevice();
    id<MTLCommandBuffer> makeNewMTLCommandBuffer();
    static Core::SharedPtr<BDCompositionDevice> Create();
    Core::SharedPtr<BDCompositionViewRenderTarget> makeViewRenderTarget(Layer *layer);
    
    Core::SharedPtr<MTLBDCompositionViewRenderTarget> makeCALayerRenderTarget(Core::Rect & rect);
    
    Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect &size);
    Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img);
    Core::SharedPtr<BDCompositionFontFactory> createFontFactory();
    Core::SharedPtr<BDCompositionVisualTree> createVisualTree();
    void renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> &visualTree, ViewRenderTarget *view,bool updatePass);
    void destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target);
};

};

#endif
