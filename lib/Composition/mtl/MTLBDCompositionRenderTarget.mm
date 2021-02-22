#import "MTLBDCompositionRenderTarget.h"
#include "NativePrivate/macos/CocoaUtils.h"

namespace OmegaWTK::Composition {

#define EIGHTBIT_TO_FLOATING_POINT(val) (val/255.f)
NSColor * color_to_ns_color(Color & color){
    return [NSColor colorWithRed:EIGHTBIT_TO_FLOATING_POINT(color.r) green:EIGHTBIT_TO_FLOATING_POINT(color.g) blue:EIGHTBIT_TO_FLOATING_POINT(color.b) alpha:EIGHTBIT_TO_FLOATING_POINT(color.a)];
};

id<MTLBuffer> solid_color_2d_mesh_to_mtl_vertex_buffer(Core::UniquePtr<MTLBDTriangulator::SolidColor2DMesh> mesh_ptr,id<MTLDevice> device){
    auto & mesh = *mesh_ptr;
    unsigned bufferLen = (mesh.size() * 3) * sizeof(MTLBDSolidColorVertex);
    NSLog(@"Buffer Length: %iu",bufferLen);
    id<MTLBuffer> buffer = [device newBufferWithLength:bufferLen options:MTLResourceStorageModeShared];
    MTLBDSolidColorVertex *dataPtr = reinterpret_cast<MTLBDSolidColorVertex *>([buffer contents]);
    for(unsigned i = 0;i < mesh.size();i++){
        auto & tri = mesh[i];
        dataPtr[i * 3] = std::move(tri.a);
        dataPtr[(i * 3) + 1] = std::move(tri.b);
        dataPtr[(i * 3) + 2] = std::move(tri.c);
    };
    return buffer;
};

MTLBDCompositionRenderTarget::MTLBDCompositionRenderTarget(MTLBDCompositionDevice *device,Native::Cocoa::CocoaItem *native_item):device(device),native_item(native_item),clearColor(0,0){
    metalLayer = [CAMetalLayer layer];
    [metalLayer setDrawableSize:CGSizeMake(native_item->rect.dimen.minWidth,native_item->rect.dimen.minHeight)];
    metalLayer.frame = Native::Cocoa::core_rect_to_cg_rect(native_item->rect);
    metalLayer.device = device->metal_device;
    OmegaWTKCocoaView *nativeView = (OmegaWTKCocoaView *)native_item->getBinding();
    [nativeView.layer addSublayer:metalLayer];
    triangulator = std::make_unique<MTLBDTriangulator>(native_item->rect);
};

Core::SharedPtr<BDCompositionRenderTarget> MTLBDCompositionRenderTarget::Create(MTLBDCompositionDevice *device,Native::Cocoa::CocoaItem *item){
    return std::make_shared<MTLBDCompositionRenderTarget>(device,item);
};

void MTLBDCompositionRenderTarget::clear(Color & clear_color){
//    NSLog(@"OmegaWTK Color: R %i G %i B %i A %i",clear_color.r,clear_color.g,clear_color.b,clear_color.a);
    clearColor = clear_color;
};

void MTLBDCompositionRenderTarget::fillRect(Core::Rect &rect,Core::SharedPtr<Brush> & brush){
    auto mesh = triangulator->triangulateToSolidColorMesh(rect);
    NSColor *rectColor = color_to_ns_color(brush->color);
    simd_float4 metalColor = {float(rectColor.redComponent),float(rectColor.greenComponent),float(rectColor.blueComponent),float(rectColor.alphaComponent)};
    for(auto & triangle : *mesh){
        triangle.c.color = triangle.b.color = triangle.a.color = metalColor;
    };
    
    id<MTLBuffer> buffer = solid_color_2d_mesh_to_mtl_vertex_buffer(std::move(mesh),device->metal_device);
    vertexBuffers.push_back(buffer);
    
    RenderPipeline rp;
    rp.pipelineState = device->solidColorPrimitive;
    rp.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Rectangle Render Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    };
    renderPasses.push(std::move(rp));
};

void MTLBDCompositionRenderTarget::frameRect(Core::Rect &rect,Core::SharedPtr<Brush> & brush,unsigned width){
    
};

void MTLBDCompositionRenderTarget::fillRoundedRect(Core::RoundedRect & rect, Core::SharedPtr<Brush> & brush){
    
};

void MTLBDCompositionRenderTarget::frameRoundedRect(Core::RoundedRect & rect, Core::SharedPtr<Brush> & brush, unsigned width){
    
};

void MTLBDCompositionRenderTarget::drawText(Core::SharedPtr<BDCompositionFont> & font, Core::String & string, Core::Rect &textRect, Core::SharedPtr<Brush> & brush){
    
};

Core::SharedPtr<BDCompositionImage> MTLBDCompositionRenderTarget::createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> & img, Core::Rect & newSize,unsigned v_id){
    
};

void MTLBDCompositionRenderTarget::drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos){
    
};

void MTLBDCompositionRenderTarget::commit(){
//    for(auto & buffer : commandBuffers){
//        [buffer waitUntilCompleted];
//    };
    
    id<CAMetalDrawable> drawable = [metalLayer nextDrawable];
    
    id<MTLCommandBuffer> finalCommandBuffer = device->makeNewMTLCommandBuffer();
    /// Clear the Screen!
    {
        NSColor *nscolor = color_to_ns_color(clearColor);
        NSLog(@"Clear Color %@",nscolor);
        MTLRenderPassDescriptor *initialRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
        initialRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
        initialRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
        initialRenderPassDesc.renderTargetWidth = native_item->rect.dimen.minWidth;
        initialRenderPassDesc.renderTargetHeight = native_item->rect.dimen.minHeight;
        initialRenderPassDesc.renderTargetArrayLength = 1;
        initialRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
//        initialRenderPassDesc.defaultRasterSampleCount = 0;
        initialRenderPassDesc.colorAttachments[0].texture = drawable.texture;
        
        MTLRenderPassDescriptor *mainRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
//        mainRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
        mainRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
        mainRenderPassDesc.renderTargetWidth = native_item->rect.dimen.minWidth;
        mainRenderPassDesc.renderTargetHeight = native_item->rect.dimen.minHeight;
        mainRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
        mainRenderPassDesc.renderTargetArrayLength = 1;
        mainRenderPassDesc.defaultRasterSampleCount = 0;
        mainRenderPassDesc.colorAttachments[0].texture = drawable.texture;
        
        id<MTLRenderCommandEncoder> initialRenderPass = [finalCommandBuffer renderCommandEncoderWithDescriptor:initialRenderPassDesc];
        [initialRenderPass endEncoding];
        unsigned idx = 0;
        while(!renderPasses.empty()){
            auto renderPass = renderPasses.front();
            renderPasses.pop();

            id<MTLRenderCommandEncoder> rp = [finalCommandBuffer renderCommandEncoderWithDescriptor:mainRenderPassDesc];
            [rp setRenderPipelineState:renderPass.pipelineState];
            renderPass.setupCallback(rp,idx);
            [rp endEncoding];
            ++idx;
        };
    }
    
    [finalCommandBuffer presentDrawable:drawable];
    [finalCommandBuffer commit];
    [metalLayer setNeedsDisplay];
//    [drawable release];
};

};
