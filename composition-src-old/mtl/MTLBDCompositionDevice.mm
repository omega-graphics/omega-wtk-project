
#import "MTLBDCompositionDevice.h"
#import "MTLBackend.h"
#import "MTLBDCompositionRenderTarget.h"
#import "MTLBDCompositionImage.h"
#import "MTLBDCALayerTree.h"

#import <dispatch/dispatch.h>

#include "NativePrivate/macos/CocoaItem.h"

#include "omegaWTK/Composition/ViewRenderTarget.h"

namespace OmegaWTK::Composition {

MTLBDCompositionDevice::MTLBDCompositionDevice(){
//    [semaphore retain];
    metal_device = MTLCreateSystemDefaultDevice();
    metal_default_library = [metal_device newDefaultLibrary];
    /**
     Setup Pipeline States
     */
    /// Kernel Funcs
    
    linearGradient = [metal_default_library newFunctionWithName:@"linearGradientKernel"];
    radialGradient = [metal_default_library newFunctionWithName:@"radialGradientKernel"];
    alphaMask = [metal_default_library newFunctionWithName:@"alphaMaskKernel"];
    
    /// Render Funcs
    solidColorVertex = [metal_default_library newFunctionWithName:@"solidColorVertex"];
    solidColorFragment = [metal_default_library newFunctionWithName:@"solidColorFragment"];
    
    texture2DVertex = [metal_default_library newFunctionWithName:@"texture2DVertex"];
    texture2DFragment = [metal_default_library newFunctionWithName:@"texture2DFragment"];
    texture2DFragmentWithBkgrd = [metal_default_library newFunctionWithName:@"texture2DFragmentWithBkgrd"];
    
    /// Kernel Pipeline States
    linearGradientKernel = setupComputePipelineState(linearGradient);
    radialGradientKernel = setupComputePipelineState(radialGradient);
    alphaMaskKernel = setupComputePipelineState(alphaMask);
    
    /// Render Pipeline States
    
    solidColorPrimitive = setupRenderPipelineState(solidColorVertex,solidColorFragment,MTLPixelFormatBGRA8Unorm);
    texture2DPrimitive = setupRenderPipelineState(texture2DVertex,texture2DFragment,MTLPixelFormatBGRA8Unorm);
    texture2DPrimitiveWithBkgrd = setupRenderPipelineState(texture2DVertex,texture2DFragmentWithBkgrd,MTLPixelFormatBGRA8Unorm);
};

inline id<MTLRenderPipelineState> MTLBDCompositionDevice::setupRenderPipelineState(id<MTLFunction> vertexFunc,id<MTLFunction> fragmentFunc,MTLPixelFormat pixelFormat){
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

inline id<MTLComputePipelineState> MTLBDCompositionDevice::setupComputePipelineState(id<MTLFunction> computeFunc){
//    MTLComputePipelineDescriptor *desc = [[MTLComputePipelineDescriptor alloc] init];
//    desc.computeFunction = computeFunc;
    NSError *error;
    auto rc = [metal_device newComputePipelineStateWithFunction:computeFunc error:&error];
    if(error.code >= 0){
        return rc;
    }
    else {
        NSLog(@"Failed to Create Compute Pipeline State!");
        return nil;
    };
};

id<MTLCommandBuffer> MTLBDCompositionDeviceContext::makeNewMTLCommandBuffer(){
    ++bufferCount;
//    return [latest commandBuffer];
    auto rc = [metal_command_queue commandBuffer];
    [buffers addObject:rc];
    return rc;
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
        return nil;
    };
};

Core::SharedPtr<BDCompositionDevice> MTLBDCompositionDevice::Create(){
    return std::make_shared<MTLBDCompositionDevice>();
    
};

Core::SharedPtr<BDCompositionDevice> CreateMTLBDCompositonDevice(){
    return MTLBDCompositionDevice::Create();
};

Core::SharedPtr<BDCompositionDeviceContext> MTLBDCompositionDevice::createContext(){
    return MTLBDCompositionDeviceContext::Create(this);
};

MTLBDCompositionDeviceContext::MTLBDCompositionDeviceContext(MTLBDCompositionDevice *device):device(device){
    metal_command_queue = [device->metal_device newCommandQueue];
    bufferCount = 0;
    buffers = [[NSMutableArray alloc] init];
//    events.push([device->metal_device newEvent]);
//    id<MTLCommandBuffer> initialSignalBuffer = makeNewMTLCommandBuffer();
////    [initialSignalBuffer encodeSignalEvent:currentEvent() value:bufferCount + 1];
//    [initialSignalBuffer enqueue];
};

Core::SharedPtr<BDCompositionDeviceContext> MTLBDCompositionDeviceContext::Create(MTLBDCompositionDevice * device){
    return std::make_shared<MTLBDCompositionDeviceContext>(device);
};

id<MTLFence> MTLBDCompositionDeviceContext::makeFence(){
    id<MTLFence> fence = [device->metal_device newFence];
    fences.push(fence);
    return fence;
};

id<MTLEvent> MTLBDCompositionDeviceContext::makeEvent(){
    id<MTLEvent> event = [device->metal_device newEvent];
    events.push(event);
    return event;
};

void MTLBDCompositionDeviceContext::freeFences(){
    while(!fences.empty()){
        auto fence = fences.front();
        fences.pop();
        [fence release];
    };
};

void MTLBDCompositionDeviceContext::freeEvents(){
    while(!events.empty()){
        auto event = events.front();
        events.pop();
        [event release];
    };
};

id<MTLEvent> MTLBDCompositionDeviceContext::currentEvent(){
    return events.back();
};

id<MTLEvent> MTLBDCompositionDeviceContext::eventAtIndex(unsigned idx){
    return nil;
};

MTLBDCompositionDevice * MTLBDCompositionDeviceContext::getParentDevice(){ return device;};

Core::SharedPtr<BDCompositionViewRenderTarget> MTLBDCompositionDeviceContext::makeViewRenderTarget(Layer *layer){
//    return MTLBDCompositionViewRenderTarget::Create(this,(Native::Cocoa::CocoaItem *)layer->getTargetNativePtr());
};

Core::SharedPtr<MTLBDCompositionViewRenderTarget> MTLBDCompositionDeviceContext::makeCALayerRenderTarget(Core::Rect & rect){
    return MTLBDCompositionViewRenderTarget::Create(this,rect);
};

Core::SharedPtr<MTLBDCompositionViewRenderTarget> MTLBDCompositionDeviceContext::makeCALayerRenderTarget(CAMetalLayer *layer,Core::Rect & rect){
    return MTLBDCompositionViewRenderTarget::CreateWithExistingCAMetalLayer(this,layer,rect);
};

Core::SharedPtr<BDCompositionImageRenderTarget> MTLBDCompositionDeviceContext::makeImageRenderTarget(Core::Rect & size){
    Core::Rect r = size;
    CGFloat scaleFactor =  [NSScreen mainScreen].backingScaleFactor;
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:int(r.dimen.minWidth *= scaleFactor) height:int(r.dimen.minHeight *= scaleFactor) mipmapped:NO];
    desc.usage = MTLTextureUsageRenderTarget | MTLTextureUsagePixelFormatView  | MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
    // desc.storageMode = MTLStorageModeShared;
    r.pos.x *= scaleFactor;
    r.pos.y *= scaleFactor;
    id<MTLTexture> target = [device->metal_device newTextureWithDescriptor:desc];
    [desc retain];
    return MTLBDCompositionImageRenderTarget::Create(this,r,target,desc);
};

Core::SharedPtr<BDCompositionImageRenderTarget> MTLBDCompositionDeviceContext::makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> & img){
    MTLBDCompositionImage *mtl_img = (MTLBDCompositionImage *)img.get();
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:mtl_img->img.width height:mtl_img->img.height mipmapped:NO];
    desc.usage = MTLTextureUsageRenderTarget | MTLTextureUsagePixelFormatView | MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
    id<MTLTexture> target = [device->metal_device newTextureWithDescriptor:desc];
    id<MTLCommandBuffer> commandBuffer = makeNewMTLCommandBuffer();
    id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
    [blitEncoder copyFromTexture:mtl_img->img toTexture:target];
    [blitEncoder endEncoding];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    return MTLBDCompositionImageRenderTarget::Create(this,mtl_img->n_rect,target,desc);
};

//Core::SharedPtr<BDCompositionFontFactory> MTLBDCompositionDevice::createFontFactory(){
//    return MTLBDCompositionFontFactory::Create();
//};

Core::SharedPtr<BDCompositionVisualTree> MTLBDCompositionDeviceContext::createVisualTree(){
    return MTLBDCALayerTree::Create(this);
};

void MTLBDCompositionDeviceContext::renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> & visualTree,ViewRenderTarget *renderTarget,bool updatePass){
        
        NSLog(@"Buffer Count:%lu",buffers.count);
        for(id<MTLCommandBuffer> buffer in buffers){
            // [buffer addCompletedHandler:^(id<MTLCommandBuffer> _buffer){
            //     auto idx = [buffers indexOfObject:_buffer];
            //     NSLog(@"Buffer %lu has Finished.",(unsigned long)idx);
            // }];
            [buffer commit];
            // [buffer waitUntilScheduled];
        };
    
        
        NSLog(@"Ready to Show");
        
        auto cocoaItem = (Native::Cocoa::CocoaItem *)renderTarget->getNativePtr();
        CALayer *viewLayer = cocoaItem->getLayer();
    //    viewLayer.bounds = Native::Cocoa::core_rect_to_cg_rect(cocoaItem->rect);
        MTLBDCALayerTree *caLayerTree = (MTLBDCALayerTree *)visualTree.get();
        MTLBDCALayerTree::Visual *root = (MTLBDCALayerTree::Visual *)caLayerTree->root_v.get();
        MTLBDCompositionImage *img = (MTLBDCompositionImage *)root->img.get();
    //    if(root->attachTransformLayer) {
    //        [viewLayer addSublayer:root->transformLayer];
    //    }
    //    else {
        // if(updatePass){
        //     [viewLayer setSublayers:@[]];
        // };
        if(!updatePass){
                [viewLayer addSublayer:root->metalLayer];
                root->metalLayer.anchorPoint = CGPointMake(0.0,0.0);
                root->metalLayer.position = CGPointMake(root->pos.x,root->pos.y);
        //        NSLog(@"Opacity Shit:%f",root->metalLayer.opacity);
                [root->metalLayer setNeedsDisplayOnBoundsChange:YES];
                [root->metalLayer setNeedsDisplay];
                [root->metalLayer setNeedsLayout];
        //    }
            
            NSLog(@"View Layer's Pos: {x:%f ,y:%f}",viewLayer.position.x,viewLayer.position.y);
            NSLog(@"Metal Layer's Pos: {x:%f ,y:%f}",root->metalLayer.position.x,root->metalLayer.position.y);
            auto visual_it = caLayerTree->body.begin();
            while(visual_it != caLayerTree->body.end()){
                auto _v = (MTLBDCALayerTree::Visual *)visual_it->get();
                //  if(!updatePass){
                    [root->metalLayer addSublayer:_v->metalLayer];
                 
                //  else {
                 
                //  }
                _v->metalLayer.anchorPoint = CGPointMake(0.0,0.0);
                _v->metalLayer.position = CGPointMake(_v->pos.x,_v->pos.y);
                NSLog(@"View Layer's Pos: {x:%f ,y:%f}",root->metalLayer.position.x,root->metalLayer.position.y);
                NSLog(@"Metal Layer's Pos: {x:%f ,y:%f}",_v->metalLayer.position.x,_v->metalLayer.position.y);
                NSLog(@"Layer Rect: {x:%f,y:%f,w:%f,h:%f",_v->metalLayer.bounds.origin.x,_v->metalLayer.bounds.origin.y,_v->metalLayer.bounds.size.width,_v->metalLayer.bounds.size.height);
                ++visual_it;
                [_v->metalLayer setNeedsDisplay];
                [_v->metalLayer setNeedsLayout];
        //        [_v->metalLayer layoutIfNeeded];
                NSLog(@"SuperLayer: %@",_v->metalLayer.superlayer);
            };
        }
        else {
            caLayerTree->layout();
        }
        
    //    [viewLayer setNeedsLayout];
       [viewLayer setNeedsDisplay];
    //    [viewLayer setContentsScale:[NSScreen mainScreen].backingScaleFactor];
        /// Reset Buffer Count after final Commit
        // }

    [buffers removeAllObjects];
    bufferCount = 0;
};

void MTLBDCompositionDeviceContext::destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target){
    
};

};

