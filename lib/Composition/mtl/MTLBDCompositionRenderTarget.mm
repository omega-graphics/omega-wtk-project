#import "MTLBDCompositionRenderTarget.h"
#include "NativePrivate/macos/CocoaUtils.h"
#import "MTLBDCompositionImage.h"
#import "MTLBDCompositionFontFactory.h"

#include <iostream>

namespace OmegaWTK::Composition {

#define EIGHTBIT_TO_FLOATING_POINT(val) (val/255.f)
NSColor * color_to_ns_color(Color & color){
    return [NSColor colorWithRed:EIGHTBIT_TO_FLOATING_POINT(color.r) green:EIGHTBIT_TO_FLOATING_POINT(color.g) blue:EIGHTBIT_TO_FLOATING_POINT(color.b) alpha:EIGHTBIT_TO_FLOATING_POINT(color.a)];
};

id<MTLBuffer> solid_color_2d_mesh_to_mtl_vertex_buffer(MTLBDTriangulator::SolidColor2DMesh * mesh_ptr,id<MTLDevice> device){
    auto & mesh = *mesh_ptr;
    unsigned bufferLen = (mesh.size() * 3) * sizeof(MTLBDSolidColorVertex);
    NSLog(@"Buffer Length: %i",bufferLen);
    id<MTLBuffer> buffer = [device newBufferWithLength:bufferLen options:MTLResourceStorageModeShared];
    MTLBDSolidColorVertex *dataPtr = reinterpret_cast<MTLBDSolidColorVertex *>([buffer contents]);
    for(unsigned i = 0;i < mesh.size();i++){
        auto & tri = mesh[i];
        dataPtr[i * 3] = std::move(tri.a);
        dataPtr[(i * 3) + 1] = std::move(tri.b);
        dataPtr[(i * 3) + 2] = std::move(tri.c);
    };
//    delete mesh_ptr;
    return buffer;
};

id<MTLBuffer> textured_mesh_to_mtl_vertex_buffer(MTLBDTriangulator::Textured2DMesh * mesh_ptr,id<MTLDevice> device){
    auto & mesh = *mesh_ptr;
    unsigned bufferLen = (mesh.size() * 3) * sizeof(MTLBDTexture2DVertex);
    NSLog(@"Buffer Length: %i bytes , triangles:%i",bufferLen,int(mesh.size()));
    id<MTLBuffer> buffer = [device newBufferWithLength:bufferLen options:MTLResourceStorageModeShared];
    MTLBDTexture2DVertex *dataPtr = reinterpret_cast<MTLBDTexture2DVertex *>([buffer contents]);
    for(unsigned i = 0;i < mesh.size();i++){
        auto & tri = mesh[i];
        dataPtr[i * 3] = std::move(tri.a);
        dataPtr[(i * 3) + 1] = std::move(tri.b);
        dataPtr[(i * 3) + 2] = std::move(tri.c);
    };
//    delete mesh_ptr;
    return buffer;
};

id<MTLTexture> make_gradient_texture_and_mask(id<MTLTexture> main,
                                     Gradient &gradientParams,
                                     MTLBDCompositionDeviceContext *deviceContext){
    @autoreleasepool {
        auto device = deviceContext->getParentDevice();
        
        /// Gradient Texture/Final Texture Descriptor
        MTLTextureDescriptor *textureDesc = [[MTLTextureDescriptor alloc] init];
        textureDesc.width = main.width;
        textureDesc.height = main.height;
        textureDesc.textureType = MTLTextureType2D;
        textureDesc.usage = MTLTextureUsagePixelFormatView | MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
        textureDesc.storageMode = MTLStorageModeShared;
        textureDesc.pixelFormat = MTLPixelFormatBGRA8Unorm;
        
        id<MTLTexture> firstRes = [device->metal_device newTextureWithDescriptor:textureDesc];
        id<MTLTexture> secondRes = [device->metal_device newTextureWithDescriptor:textureDesc];
        
        Core::Vector<MTLBDGradientStop> _stops;
        
        auto gradient_stop_it = gradientParams.stops.begin();
        while(gradient_stop_it != gradientParams.stops.end()){
            auto nsColor = color_to_ns_color(gradient_stop_it->color);
            _stops.push_back({gradient_stop_it->pos,{float(nsColor.redComponent),float(nsColor.greenComponent),float(nsColor.blueComponent),float(nsColor.alphaComponent)}});
            [nsColor release];
            ++gradient_stop_it;
        };
        
        id<MTLBuffer> gradientStopBuffer = [device->metal_device newBufferWithBytes:_stops.data() length:_stops.size() * sizeof(MTLBDGradientStop) options:MTLResourceStorageModeShared];
        
        
        
        id<MTLCommandBuffer> computeBuffer = deviceContext->makeNewMTLCommandBuffer();
        id<MTLComputeCommandEncoder> computeEncoder0 = [computeBuffer computeCommandEncoder];
        id<MTLComputePipelineState> computePipelineState;
        /// Make Gradient Texture!
        if(gradientParams.type == Gradient::Linear){
            computePipelineState = device->linearGradientKernel;
        }
        else if(gradientParams.type == Gradient::Radial){
            computePipelineState = device->radialGradientKernel;
        }
        else {
            computePipelineState = nil;
        };
        
        id<MTLFence> computeFence = deviceContext->makeFence();
        
        [computeEncoder0 setComputePipelineState:computePipelineState];
        [computeEncoder0 setBuffer:gradientStopBuffer offset:0 atIndex:OMEGAWTK_METAL_GRADIENT_STOP_BUFFER];
        [computeEncoder0 setTexture:firstRes atIndex:OMEGAWTK_METAL_GRADIENT_TEXTURE_OUT];
        [computeEncoder0 updateFence:computeFence];
        [computeEncoder0 endEncoding];
        
        id<MTLComputeCommandEncoder> computeEncoder1 = [computeBuffer computeCommandEncoder];
        [computeEncoder1 waitForFence:computeFence];
        [computeEncoder1 setComputePipelineState:device->alphaMaskKernel];
        [computeEncoder1 setTexture:firstRes atIndex:OMEGAWTK_METAL_MASK_TEXTURE_MAIN];
        [computeEncoder1 setTexture:main atIndex:OMEGAWTK_METAL_MASK_TEXTURE_MASK];
        [computeEncoder1 setTexture:secondRes atIndex:OMEGAWTK_METAL_MASK_TEXTURE_OUT];
        [computeEncoder1 endEncoding];
        [computeBuffer enqueue];
        
        return secondRes;
    };
    
};


Core::SharedPtr<MTLBDCompositionViewRenderTarget> MTLBDCompositionViewRenderTarget::Create(MTLBDCompositionDeviceContext *deviceContext,Core::Rect & rect){
    return std::make_shared<MTLBDCompositionViewRenderTarget>(deviceContext,rect);
};
/// Metal Composition Render Target!

MTLBDCompositionViewRenderTarget::MTLBDCompositionViewRenderTarget(MTLBDCompositionDeviceContext *deviceContext,Core::Rect & _rect):MTLBDCompositionRenderTarget(deviceContext,Color(0,0,0,0),_rect),rect(_rect){
    triangulator->setScaleFactor(1);
    metalLayer = [CAMetalLayer layer];
//    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    auto rect = Native::Cocoa::core_rect_to_cg_rect(_rect);
//    rect.origin.x /= scaleFactor;
//    rect.origin.y /= scaleFactor;
    
    metalLayer.frame = rect;
    metalLayer.bounds = CGRectMake(0,0,rect.size.width,rect.size.height);
//    metalLayer.bounds = CGRectMake(0,0,rect.size.width,rect.size.height);
    metalLayer.device = deviceContext->getParentDevice()->metal_device;
    metalLayer.opaque = NO;
    metalLayer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
//    metalLayer.presentsWithTransaction = YES;
    NSLog(@"Position: x%f, y%f",metalLayer.frame.origin.x,metalLayer.frame.origin.y);
    metalLayer.contentsScale = 2.f;
    metalLayer.masksToBounds = YES;
//    metalLayer.contentsCenter = CGRectMake(0,0,1,1);
    metalLayer.framebufferOnly = YES;
//    metalLayer.allowsNextDrawableTimeout = YES;
};

void MTLBDCompositionViewRenderTarget::clear(Color & clear_color){
    clearColor = std::move(clear_color);
};

void MTLBDCompositionViewRenderTarget::commit(){
//    NSLog(@"Commit!!!");
//        NSLog(@"Commit!!!");
    auto device = deviceContext->getParentDevice();
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
    @autoreleasepool {
        
    
        
        NSLog(@"Waiting for next Drawable");
        currentDrawable = [metalLayer nextDrawable];
        if(currentDrawable != nil) {
            id<MTLCommandBuffer> finalCommandBuffer = deviceContext->makeNewMTLCommandBuffer();
//            [finalCommandBuffer encodeWaitForEvent:deviceContext->currentEvent() value:deviceContext->bufferCount];
            /// Clear the Screen!
    //            NSColor *nscolor = color_to_ns_color(clearColor);
                NSColor *nscolor = color_to_ns_color(clearColor);
                NSLog(@"Clear Color %@",nscolor);
                MTLRenderPassDescriptor *initialRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
                initialRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
                initialRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
                initialRenderPassDesc.renderTargetWidth = float(rect.dimen.minWidth) * 2.f;
                initialRenderPassDesc.renderTargetHeight = float(rect.dimen.minHeight) * 2.f;
                initialRenderPassDesc.renderTargetArrayLength = 1;
    //            initialRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
    //            initialRenderPassDesc.defaultRasterSampleCount = 0;
                initialRenderPassDesc.colorAttachments[0].texture = currentDrawable.texture;
                
                MTLRenderPassDescriptor *mainRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
                mainRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
                mainRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
                mainRenderPassDesc.renderTargetWidth = float(rect.dimen.minWidth) * 2.f;
                mainRenderPassDesc.renderTargetHeight = float(rect.dimen.minHeight) * 2.f;
                mainRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
                mainRenderPassDesc.renderTargetArrayLength = 1;
//                mainRenderPassDesc.defaultRasterSampleCount = 0;
                mainRenderPassDesc.colorAttachments[0].texture = currentDrawable.texture;
                
//                MTLRenderPassDescriptor *multiSampledRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
//                multiSampledRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
//                multiSampledRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
//                multiSampledRenderPassDesc.colorAttachments[0].resolveTexture = currentDrawable.texture;
//                multiSampledRenderPassDesc.renderTargetWidth = float(native_item->rect.dimen.minWidth) * scaleFactor;
//                multiSampledRenderPassDesc.renderTargetHeight = float(native_item->rect.dimen.minHeight) * scaleFactor;
//                multiSampledRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStoreAndMultisampleResolve;
//                multiSampledRenderPassDesc.renderTargetArrayLength = 1;
                
//                MTLTextureDescriptor *multiSampleTextureDesc = [[MTLTextureDescriptor alloc] init];
//                multiSampleTextureDesc.textureType = MTLTextureType2DMultisample;
//                multiSampleTextureDesc.storageMode = MTLStorageModePrivate;
//                multiSampleTextureDesc.width = float(native_item->rect.dimen.minWidth) * scaleFactor;
//                multiSampleTextureDesc.height = float(native_item->rect.dimen.minHeight) * scaleFactor;
//                multiSampleTextureDesc.pixelFormat = MTLPixelFormatBGRA8Unorm;
//                multiSampleTextureDesc.usage = MTLTextureUsageRenderTarget;
//
                id<MTLRenderCommandEncoder> initialRenderPass = [finalCommandBuffer renderCommandEncoderWithDescriptor:initialRenderPassDesc];
                [initialRenderPass endEncoding];
                unsigned idx = 0;
                while(!renderPasses.empty()){
                    auto renderPass = renderPasses.front();
                    renderPasses.pop();
                    id<MTLRenderCommandEncoder> rp;
//                    if(renderPass.multiSampled){
//                        multiSampleTextureDesc.sampleCount = renderPass.sampleCount;
//                        id<MTLTexture> msTexture = [device->metal_device newTextureWithDescriptor:multiSampleTextureDesc];
//                        multiSampledRenderPassDesc.colorAttachments[0].texture = msTexture;
//                        rp = [finalCommandBuffer renderCommandEncoderWithDescriptor:multiSampledRenderPassDesc];
//                    }
//                    else {
                    rp = [finalCommandBuffer renderCommandEncoderWithDescriptor:mainRenderPassDesc];
//                    };
                    [rp setRenderPipelineState:renderPass.pipelineState];
                    renderPass.setupCallback(rp,idx);
                    [rp endEncoding];
                    ++idx;
                };
            NSLog(@"Going to Present Drawable");
            [finalCommandBuffer presentDrawable:currentDrawable];
            NSLog(@"Presented Drawable!");
//            [finalCommandBuffer encodeSignalEvent:deviceContext->currentEvent() value:deviceContext->bufferCount + 1];
            [finalCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer){
//                    auto buffer_it = vertexBuffers.begin();
//                    while(buffer_it != vertexBuffers.end()){
//                        id<MTLBuffer> buffer = *buffer_it;
//                        [buffer setPurgeableState:MTLPurgeableStateEmpty];
//                        ++buffer_it;
//                    };
                    vertexBuffers.clear();
                NSLog(@"Completed!");
            }];
            [finalCommandBuffer enqueue];
            NSLog(@"Enqueued Buffer");
            [metalLayer setNeedsDisplay];
            [metalLayer retain];
        };
    };
    NSLog(@"Returning");
};

/// Metal Image Render Target!

MTLBDCompositionImageRenderTarget::MTLBDCompositionImageRenderTarget(MTLBDCompositionDeviceContext * deviceContext,Core::Rect & rect,id<MTLTexture> target,MTLTextureDescriptor *desc):MTLBDCompositionRenderTarget(deviceContext,Color(0,0,0,0),rect),target(target),rect(rect),desc(desc){
    triangulator->setScaleFactor([NSScreen mainScreen].backingScaleFactor);
    triangulator->isImageTarget = true;
};

Core::SharedPtr<BDCompositionImageRenderTarget> MTLBDCompositionImageRenderTarget::Create(MTLBDCompositionDeviceContext *deviceContext,Core::Rect & rect,id<MTLTexture> texture,MTLTextureDescriptor *desc){
    return std::make_shared<MTLBDCompositionImageRenderTarget>(deviceContext,rect,texture,desc);
};

void MTLBDCompositionImageRenderTarget::commit(){
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    @autoreleasepool {
//        for(auto & buffer : commandBuffers){
//            [buffer waitUntilCompleted];
//        };
        commandBuffers.clear();
        NSColor *_clearColor = color_to_ns_color(clearColor);
        
        MTLRenderPassDescriptor *renderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
        renderPassDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
        renderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(_clearColor.redComponent,_clearColor.greenComponent,_clearColor.blueComponent,_clearColor.alphaComponent);
        renderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
        renderPassDesc.colorAttachments[0].texture = target;
        renderPassDesc.defaultRasterSampleCount = 1;
        renderPassDesc.renderTargetWidth = target.width;
        renderPassDesc.renderTargetHeight = target.height;
        renderPassDesc.renderTargetArrayLength = 1;
        
        MTLRenderPassDescriptor *renderPassDesc2 = [MTLRenderPassDescriptor renderPassDescriptor];
        renderPassDesc2.colorAttachments[0].loadAction = MTLLoadActionLoad;
//        renderPassDesc2.colorAttachments[0].clearColor = MTLClearColorMake(_clearColor.redComponent,_clearColor.greenComponent,_clearColor.blueComponent,_clearColor.alphaComponent);
        renderPassDesc2.colorAttachments[0].storeAction = MTLStoreActionStore;
        renderPassDesc2.colorAttachments[0].texture = target;
        renderPassDesc2.defaultRasterSampleCount = 1;
        renderPassDesc2.renderTargetWidth = target.width;
        renderPassDesc2.renderTargetHeight = target.height;
        renderPassDesc2.renderTargetArrayLength = 1;
        id<MTLCommandBuffer> finalCommandBuffer = deviceContext->makeNewMTLCommandBuffer();
//        [finalCommandBuffer encodeWaitForEvent:deviceContext->currentEvent() value:deviceContext->bufferCount];
        id<MTLRenderCommandEncoder> clearRp = [finalCommandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
        [clearRp endEncoding];
        unsigned idx = 0;
        while(!renderPasses.empty()){
            auto renderPass = renderPasses.front();
            renderPasses.pop();
            id<MTLRenderCommandEncoder> rp = [finalCommandBuffer renderCommandEncoderWithDescriptor:renderPassDesc2];
            [rp setRenderPipelineState:renderPass.pipelineState];
            renderPass.setupCallback(rp,idx);
            [rp endEncoding];
            ++idx;
        };
        
//        [finalCommandBuffer encodeSignalEvent:deviceContext->currentEvent() value:deviceContext->bufferCount + 1];
        
        [finalCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer){
            if(buffer.error.code < 0){
                NSLog(@"Buffer failed to excute Code:%i",buffer.error.code);
            }
            else {
                NSLog(@"Image has Finished");
                vertexBuffers.clear();
            };
        }];
        [finalCommandBuffer enqueue];
//        [finalCommandBuffer waitUntilCompleted];
    }
};

Core::SharedPtr<BDCompositionImage> MTLBDCompositionImageRenderTarget::getImg(){
    return MTLBDCompositionImage::Create(deviceContext,rect,target,desc);
};

};
