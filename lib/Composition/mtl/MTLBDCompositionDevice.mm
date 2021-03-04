
#import "MTLBDCompositionDevice.h"
#import "MTLBackend.h"
#import "MTLBDCompositionRenderTarget.h"
#import "MTLBDCompositionFontFactory.h"
#import "MTLBDCompositionImage.h"
#import "MTLBDCALayerTree.h"

#include "NativePrivate/macos/CocoaItem.h"

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
    solidColorVertex = [metal_default_library newFunctionWithName:@"solidColorVertex"];
    solidColorFragment = [metal_default_library newFunctionWithName:@"solidColorFragment"];
    
    texture2DVertex = [metal_default_library newFunctionWithName:@"texture2DVertex"];
    texture2DFragment = [metal_default_library newFunctionWithName:@"texture2DFragment"];
    texture2DFragmentWithBkgrd = [metal_default_library newFunctionWithName:@"texture2DFragmentWithBkgrd"];
    
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

id<MTLRenderPipelineState> MTLBDCompositionDevice::makeMultiSampledPipelineState(bool textured,unsigned sampleCount,NSString *label){
    MTLRenderPipelineDescriptor *desc = [[MTLRenderPipelineDescriptor alloc] init];
    desc.label = label;
    if(textured){
        desc.vertexFunction = texture2DVertex;
        desc.fragmentFunction = texture2DFragment;
    }
    else {
        desc.vertexFunction = solidColorVertex;
        desc.fragmentFunction = solidColorFragment;
    };
    desc.sampleCount = sampleCount;
    desc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
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
        NSLog(@"Failed to Create Mutli Sampled Render Pipeline State:%@",error);
    };
};

Core::SharedPtr<BDCompositionDevice> MTLBDCompositionDevice::Create(){
    return std::make_shared<MTLBDCompositionDevice>();
    
};

Core::SharedPtr<BDCompositionDevice> CreateMTLBDCompositonDevice(){
    return MTLBDCompositionDevice::Create();
};

Core::SharedPtr<BDCompositionViewRenderTarget> MTLBDCompositionDevice::makeViewRenderTarget(Layer *layer){
//    return MTLBDCompositionViewRenderTarget::Create(this,(Native::Cocoa::CocoaItem *)layer->getTargetNativePtr());
};

Core::SharedPtr<MTLBDCompositionViewRenderTarget> MTLBDCompositionDevice::makeCALayerRenderTarget(Core::Rect & rect){
    return MTLBDCompositionViewRenderTarget::Create(this,rect);
};

Core::SharedPtr<BDCompositionImageRenderTarget> MTLBDCompositionDevice::makeImageRenderTarget(Core::Rect & size){
    float scaleFactor =  [NSScreen mainScreen].backingScaleFactor;
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:int(size.dimen.minWidth *= scaleFactor) height:int(size.dimen.minHeight *= scaleFactor) mipmapped:NO];
    desc.usage = MTLTextureUsageRenderTarget | MTLTextureUsagePixelFormatView | MTLTextureUsageShaderRead;
//    desc.storageMode = MTLStorageModeShared;
    size.pos.x *= scaleFactor;
    size.pos.y *= scaleFactor;
    id<MTLTexture> target = [metal_device newTextureWithDescriptor:desc];
    return MTLBDCompositionImageRenderTarget::Create(this,size,target);
};

Core::SharedPtr<BDCompositionImageRenderTarget> MTLBDCompositionDevice::makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> & img){
    MTLBDCompositionImage *mtl_img = (MTLBDCompositionImage *)img.get();
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:mtl_img->img.width height:mtl_img->img.height mipmapped:NO];
    desc.usage = MTLTextureUsageRenderTarget | MTLTextureUsagePixelFormatView | MTLTextureUsageShaderRead;
    id<MTLTexture> target = [metal_device newTextureWithDescriptor:desc];
    id<MTLCommandBuffer> commandBuffer = makeNewMTLCommandBuffer();
    id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
    [blitEncoder copyFromTexture:mtl_img->img toTexture:target];
    [blitEncoder endEncoding];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    return MTLBDCompositionImageRenderTarget::Create(this,mtl_img->n_rect,target);
};

Core::SharedPtr<BDCompositionFontFactory> MTLBDCompositionDevice::createFontFactory(){
    return MTLBDCompositionFontFactory::Create();
};

Core::SharedPtr<BDCompositionVisualTree> MTLBDCompositionDevice::createVisualTree(){
    return MTLBDCALayerTree::Create(this);
};

void MTLBDCompositionDevice::renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> & visualTree,ViewRenderTarget *renderTarget){
    auto cocoaItem = (Native::Cocoa::CocoaItem *)renderTarget->getNativePtr();
    CALayer *viewLayer = cocoaItem->getLayer();
//    viewLayer.bounds = Native::Cocoa::core_rect_to_cg_rect(cocoaItem->rect);
    MTLBDCALayerTree *caLayerTree = (MTLBDCALayerTree *)visualTree.get();
    MTLBDCALayerTree::Visual *root = (MTLBDCALayerTree::Visual *)caLayerTree->root_v.get();
    MTLBDCompositionImage *img = (MTLBDCompositionImage *)root->img.get();
    if(root->attachTransformLayer) {
        [viewLayer addSublayer:root->transformLayer];
    }
    else {
        [viewLayer addSublayer:root->metalLayer];
        root->metalLayer.position = CGPointMake(root->metalLayer.bounds.size.width/2,root->metalLayer.bounds.size.height/2);
    }
    
    NSLog(@"View Layer's Pos: {x:%f ,y:%f}",viewLayer.position.x,viewLayer.position.y);
    NSLog(@"Metal Layer's Pos: {x:%f ,y:%f}",root->metalLayer.position.x,root->metalLayer.position.y);
//    [viewLayer setContentsScale:[NSScreen mainScreen].backingScaleFactor];
    [viewLayer setNeedsDisplay];
};

void MTLBDCompositionDevice::destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target){
    
};

};

