#include "../BDCompositionDevice.h"
#include "../BDCompositionRenderTarget.h"

#import <Metal/Metal.h>

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONDEVICE_H

@class CAMetalLayer;

namespace OmegaWTK::Composition {

class MTLBDCompositionViewRenderTarget;
class MTLBDCompositionDeviceContext;
/**
 Metal Composition Device
 */
class MTLBDCompositionDevice : public BDCompositionDevice {
public:
    id<MTLDevice> metal_device;
    id<MTLLibrary> metal_default_library;
    /**
     Pipeline States
     ===
     */
    /**
     Render Pipelnes
     */
    id<MTLRenderPipelineState> solidColorPrimitive;
    id<MTLRenderPipelineState> texture2DPrimitive;
    id<MTLRenderPipelineState> texture2DPrimitiveWithBkgrd;
    /**
     Compute Pipelines
     */
    id<MTLComputePipelineState> linearGradientKernel;
    id<MTLComputePipelineState> radialGradientKernel;
    id<MTLComputePipelineState> alphaMaskKernel;
    /**
     Metal Library Functions
     ===
     */
    /**
     Kernel Functions
     */
    id<MTLFunction> linearGradient;
    id<MTLFunction> radialGradient;
    id<MTLFunction> alphaMask;
    /**
     Shading Functions
     */
    id<MTLFunction> solidColorVertex;
    id<MTLFunction> solidColorFragment;
    id<MTLFunction> texture2DVertex;
    id<MTLFunction> texture2DFragment;
    id<MTLFunction> texture2DFragmentWithBkgrd;
private:
//    Core::Vector<id<MTLCommandQueue>> metal_command_queues;
    inline id<MTLRenderPipelineState> setupRenderPipelineState(id<MTLFunction> vertexFunc,id<MTLFunction> fragmentFunc,MTLPixelFormat pixelFormat);
    inline id<MTLComputePipelineState> setupComputePipelineState(id<MTLFunction> computeFunc);
    friend class MTLBDCompositionDeviceContext;
public: 
    id<MTLRenderPipelineState> makeMultiSampledPipelineState(bool textured,unsigned sampleCount,NSString *label);
    MTLBDCompositionDevice();
    static Core::SharedPtr<BDCompositionDevice> Create();
    Core::SharedPtr<BDCompositionDeviceContext> createContext();
//    Core::SharedPtr<BDCompositionFontFactory> createFontFactory();
};

/**
 A Context of which the Composition Device renders to.
 */
class MTLBDCompositionDeviceContext : public BDCompositionDeviceContext {
private:
    MTLBDCompositionDevice *device;
    id<MTLCommandQueue> metal_command_queue;
    NSMutableArray<id<MTLCommandBuffer>> *buffers;
    Core::Queue<id<MTLFence>> fences;
    Core::Queue<id<MTLEvent>> events;
public:
    unsigned bufferCount = 0;
    id<MTLCommandBuffer> makeNewMTLCommandBuffer();
    id<MTLFence> makeFence();
    id<MTLEvent> makeEvent();
    id<MTLEvent> currentEvent();
    id<MTLEvent> eventAtIndex(unsigned idx);
    void freeFences();
    void freeEvents();
    MTLBDCompositionDevice * getParentDevice();
    Core::SharedPtr<BDCompositionViewRenderTarget> makeViewRenderTarget(Layer *layer);
    /**
     Creates a  CALayer Render Target from a Core::Rect (Creates a CAMetalLayer)
    */
    Core::SharedPtr<MTLBDCompositionViewRenderTarget> makeCALayerRenderTarget(Core::Rect & rect);
    /**
     Creates a  CALayer Render Target from an existing CAMetalLayer (Does NOT create nor own its own layer)
    */
    Core::SharedPtr<MTLBDCompositionViewRenderTarget> makeCALayerRenderTarget(CAMetalLayer *layer,Core::Rect & rect);
    
    Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect &size);
    Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img);
    
    Core::SharedPtr<BDCompositionVisualTree> createVisualTree();
    void renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> &visualTree, ViewRenderTarget *view,bool updatePass);
    void destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target);
    /**
     Construct a MTLBDDeviceContext.
     @param device
     @returns A shared handle a device context.
     */
    static Core::SharedPtr<BDCompositionDeviceContext> Create(MTLBDCompositionDevice *device);
    /**
     @private
     Main Constructor for class... Ignore.
     */
    MTLBDCompositionDeviceContext(MTLBDCompositionDevice *device);
};

};

#endif
