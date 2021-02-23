
#import "MTLBDCompositionDevice.h"
#import "MTLBackend.h"
#import "MTLBDCompositionRenderTarget.h"
#import "MTLBDCompositionFontFactory.h"

namespace OmegaWTK::Composition {

MTLBDCompositionDevice::MTLBDCompositionDevice(){
    metal_device = MTLCreateSystemDefaultDevice();
    metal_default_library = [metal_device newDefaultLibrary];
    metal_command_queue = [metal_device newCommandQueue];
//    metal_command_queues.push_back([metal_device newCommandQueue]);
    bufferCount = 0;
    
    /**
     Setup Pipeline States
     */
    id<MTLFunction> solidColorVertex = [metal_default_library newFunctionWithName:@"solidColorVertex"];
    id<MTLFunction> solidColorFragment = [metal_default_library newFunctionWithName:@"solidColorFragment"];
    
    id<MTLFunction> texture2DVertex = [metal_default_library newFunctionWithName:@"texture2DVertex"];
    id<MTLFunction> texture2DFragment = [metal_default_library newFunctionWithName:@"texture2DFragment"];
    id<MTLFunction> texture2DFragmentWithBkgrd = [metal_default_library newFunctionWithName:@"texture2DFragmentWithBkgrd"];
    
    solidColorPrimitive = setupPipelineState(solidColorVertex,solidColorFragment,MTLPixelFormatBGRA8Unorm);
    texture2DPrimitive = setupPipelineState(texture2DVertex,texture2DFragment,MTLPixelFormatBGRA8Unorm);
    texture2DPrimitiveWithBkgrd = setupPipelineState(texture2DVertex,texture2DFragmentWithBkgrd,MTLPixelFormatBGRA8Unorm);
};

inline id<MTLRenderPipelineState> MTLBDCompositionDevice::setupPipelineState(id<MTLFunction> vertexFunc,id<MTLFunction> fragmentFunc,MTLPixelFormat pixelFormat){
    MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
    desc.vertexFunction =  vertexFunc;
    desc.fragmentFunction = fragmentFunc;
    desc.rasterSampleCount = 1;
    desc.sampleCount = 1;
    /// Enable Alpha Blending!!!
    desc.colorAttachments[0].pixelFormat = pixelFormat;
    desc.colorAttachments[0].blendingEnabled = YES;
    desc.colorAttachments[0].alphaBlendOperation = desc.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    desc.colorAttachments[0].sourceRGBBlendFactor = desc.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    desc.colorAttachments[0].destinationRGBBlendFactor = desc.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    NSError *error;
    auto rc = [metal_device newRenderPipelineStateWithDescriptor:desc error:&error];
    if(error.code >= 0){
        return rc;
    }
    else {
        NSLog(@"Failed to Create Render Pipeline State!");
        return nil;
    };
//    return rc;
};

id<MTLCommandBuffer> MTLBDCompositionDevice::makeNewMTLCommandBuffer(){
//    id<MTLCommandQueue> latest;
//    if(bufferCount == 64){
//        latest = [metal_device newCommandQueue];
//        metal_command_queues.push_back(latest);
//        bufferCount = 0;
//    }
//    else {
//        latest = metal_command_queues.back();
//    };
//    ++bufferCount;
//    return [latest commandBuffer];
    return [metal_command_queue commandBuffer];
};

Core::SharedPtr<BDCompositionDevice> MTLBDCompositionDevice::Create(){
    return std::make_shared<MTLBDCompositionDevice>();
    
};

Core::SharedPtr<BDCompositionDevice> CreateMTLBDCompositonDevice(){
    return MTLBDCompositionDevice::Create();
};

Core::SharedPtr<BDCompositionRenderTarget> MTLBDCompositionDevice::makeTarget(Layer *layer){
    return MTLBDCompositionRenderTarget::Create(this,(Native::Cocoa::CocoaItem *)layer->getTargetNativePtr());
};

Core::SharedPtr<BDCompositionFontFactory> MTLBDCompositionDevice::createFontFactory(){
    return MTLBDCompositionFontFactory::Create();
};

void MTLBDCompositionDevice::destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target){
    
};

};

