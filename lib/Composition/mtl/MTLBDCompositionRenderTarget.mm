#import "MTLBDCompositionRenderTarget.h"
#include "NativePrivate/macos/CocoaUtils.h"
#import "MTLBDCompositionImage.h"
#import "MTLBDCompositionFontFactory.h"

#import <MetalPerformanceShaders/MetalPerformanceShaders.h>
#import <CoreImage/CoreImage.h>

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

MTLBDCompositionRenderTarget::MTLBDCompositionRenderTarget(MTLBDCompositionDevice *device,Native::Cocoa::CocoaItem *native_item):device(device),native_item(native_item),clearColor(0,0){
    metalLayer = [CAMetalLayer layer];
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    auto rect = Native::Cocoa::core_rect_to_cg_rect(native_item->rect);
    metalLayer.frame = rect;
    metalLayer.bounds = rect;
    metalLayer.device = device->metal_device;
    metalLayer.presentsWithTransaction = YES;
//    metalLayer.position = rect.origin;
    NSLog(@"Position: x%f, y%f",metalLayer.position.x,metalLayer.position.y);
    metalLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
    OmegaWTKCocoaView *nativeView = (OmegaWTKCocoaView *)native_item->getBinding();
    nativeView.layer = metalLayer;
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
    /// Ensure proper antialiased rendering on a high DPI monitor!
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
    auto floatRect = FRect(float(rect.pos.x) * scaleFactor,float(rect.pos.y) * scaleFactor, float(rect.dimen.minWidth) * scaleFactor,float(rect.dimen.minHeight) * scaleFactor);
    
    
    NSLog(@"Rect: x:%f, y:%f, w:%f, h:%f",floatRect.pos.x,floatRect.pos.y,floatRect.dimen.minWidth,floatRect.dimen.minHeight);
    
    auto mesh = triangulator->triangulateToSolidColorMesh(floatRect,false,0);
    
    NSColor *rectColor = color_to_ns_color(brush->color);
    simd_float4 metalColor = {float(rectColor.redComponent),float(rectColor.greenComponent),float(rectColor.blueComponent),float(rectColor.alphaComponent)};
    for(auto & triangle : *mesh){
        triangle.c.color = triangle.b.color = triangle.a.color = metalColor;
    };
//
    id<MTLBuffer> buffer = solid_color_2d_mesh_to_mtl_vertex_buffer(mesh.get(),device->metal_device);
    vertexBuffers.push_back(buffer);

    RenderPipeline rp;
    rp.pipelineState = device->solidColorPrimitive;
    /// V-id --> Visual Id!
    rp.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Rectangle Render Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    };
    renderPasses.push(std::move(rp));
};

void MTLBDCompositionRenderTarget::frameRect(Core::Rect &rect,Core::SharedPtr<Brush> & brush,unsigned width){
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    auto frect = FRect(float(rect.pos.x) * scaleFactor,float(rect.pos.y) * scaleFactor,float(rect.dimen.minWidth) * scaleFactor,float(rect.dimen.minHeight) * scaleFactor);
//
    NSLog(@"Rect: x:%f, y:%f, w:%f, h:%f",frect.pos.x,frect.pos.y,frect.dimen.minWidth,frect.dimen.minHeight);
    
    auto mesh = triangulator->triangulateToSolidColorMesh(frect,true,width);
    NSColor *rectColor = color_to_ns_color(brush->color);
    simd_float4 metalColor = {float(rectColor.redComponent),float(rectColor.greenComponent),float(rectColor.blueComponent),float(rectColor.alphaComponent)};
    for(auto & triangle : *mesh){
        triangle.c.color = triangle.b.color = triangle.a.color = metalColor;
    };
    id<MTLBuffer> buffer = solid_color_2d_mesh_to_mtl_vertex_buffer(mesh.get(),device->metal_device);
    vertexBuffers.push_back(buffer);
    RenderPipeline rp;
    rp.pipelineState = device->solidColorPrimitive;
    /// V-id --> Visual Id!
    rp.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Rectangle Render Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        auto v_count = vertexBuffers[v_id].length / sizeof(MTLBDSolidColorVertex);
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:v_count];
    };
    renderPasses.push(std::move(rp));
};

void MTLBDCompositionRenderTarget::fillRoundedRect(Core::RoundedRect & rect, Core::SharedPtr<Brush> & brush){
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    auto frect = FRoundedRect(float(rect.pos.x) * scaleFactor,float(rect.pos.y) * scaleFactor,float(rect.dimen.minWidth) * scaleFactor,float(rect.dimen.minHeight) * scaleFactor,float(rect.radius_x) * scaleFactor,float(rect.radius_y) * scaleFactor);
    
    auto res = triangulator->triangulateToSolidColorMeshes(frect);
    NSColor *rectColor = color_to_ns_color(brush->color);
    simd_float4 metalColor = {float(rectColor.redComponent),float(rectColor.greenComponent),float(rectColor.blueComponent),float(rectColor.alphaComponent)};
    for(auto & mesh : res){
        for(auto & triangle : *mesh)
            triangle.c.color = triangle.b.color = triangle.a.color = metalColor;
    };
    /// Rect Buffer --> 0
    vertexBuffers.push_back(solid_color_2d_mesh_to_mtl_vertex_buffer(res[0],device->metal_device));
    /// Arc Buffer --> 1
    vertexBuffers.push_back(solid_color_2d_mesh_to_mtl_vertex_buffer(res[1],device->metal_device));
    RenderPipeline rp0, rp1;
    rp1.pipelineState = rp0.pipelineState = device->solidColorPrimitive;
    /// V-id --> Visual Id!
    rp0.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Rect Render for Rounded Rect Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        auto v_count = vertexBuffers[v_id].length / sizeof(MTLBDSolidColorVertex);
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:v_count];
    };
    
    renderPasses.push(std::move(rp0));
  
    rp1.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Arc Render for Rounded Rect Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        auto v_count = vertexBuffers[v_id].length / sizeof(MTLBDSolidColorVertex);
        [encoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:v_count];
    };
    
    renderPasses.push(std::move(rp1));
};

void MTLBDCompositionRenderTarget::frameRoundedRect(Core::RoundedRect & rect, Core::SharedPtr<Brush> & brush, unsigned width){
    /// TODO: Implement!
};

void MTLBDCompositionRenderTarget::drawText(Core::SharedPtr<BDCompositionFont> & font, Core::String & string, Core::Rect &textRect, Core::SharedPtr<Brush> & brush){
    MTLBDCompositionFont *ct_font = (MTLBDCompositionFont *)font.get();
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    
    /// Initalize Text with Settings!
    [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
    NSTextAlignment hAlignment;
    NSTextBlockVerticalAlignment vAlignment;
    switch (ct_font->omegawtkFont.textAlignment) {
        case Text::Font::LeftUpper: {
            hAlignment = NSTextAlignmentLeft;
            vAlignment = NSTextBlockTopAlignment;
            break;
        }
        case Text::Font::LeftCenter : {
            hAlignment = NSTextAlignmentLeft;
            vAlignment = NSTextBlockMiddleAlignment;
            break;
        };
        case Text::Font::LeftLower : {
            hAlignment = NSTextAlignmentLeft;
            vAlignment = NSTextBlockBottomAlignment;
            break;
        };
        case Text::Font::MiddleUpper: {
            hAlignment = NSTextAlignmentCenter;
            vAlignment = NSTextBlockTopAlignment;
            break;
        }
        case Text::Font::MiddleCenter : {
            hAlignment = NSTextAlignmentCenter;
            vAlignment = NSTextBlockMiddleAlignment;
            break;
        };
        case Text::Font::MiddleLower : {
            hAlignment = NSTextAlignmentCenter;
            vAlignment = NSTextBlockBottomAlignment;
            break;
        };
        case Text::Font::RightUpper: {
            hAlignment = NSTextAlignmentRight;
            vAlignment = NSTextBlockTopAlignment;
            break;
        }
        case Text::Font::RightCenter : {
            hAlignment = NSTextAlignmentRight;
            vAlignment = NSTextBlockMiddleAlignment;
            break;
        };
        case Text::Font::RightLower : {
            hAlignment = NSTextAlignmentRight;
            vAlignment = NSTextBlockBottomAlignment;
            break;
        };
        default:
            break;
    }
    NSLineBreakMode lineBreakOpts;
    switch (ct_font->omegawtkFont.wrapping) {
        case Text::Font::WrapByWord : {
            lineBreakOpts = NSLineBreakByWordWrapping;
            break;
        }
        case Text::Font::WrapByCharacter : {
            lineBreakOpts = NSLineBreakByCharWrapping;
            break;
        };
        case Text::Font::None : {
            lineBreakOpts = NSLineBreakByClipping;
            break;
        };
        default:
            break;
    }
    
    [paragraphStyle setAlignment:hAlignment];
    [paragraphStyle setLineBreakMode:lineBreakOpts];
    for(NSTextBlock *textBlock in paragraphStyle.textBlocks){
        [textBlock setVerticalAlignment:vAlignment];
    };
    
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    auto ftextRect = FRect(float(textRect.pos.x) * scaleFactor,float(textRect.pos.y) * scaleFactor,float(textRect.dimen.minWidth) * scaleFactor,float(textRect.dimen.minHeight) * scaleFactor);
    
    // Draw Text to CGBitmap!
    
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)[[NSAttributedString alloc] initWithString:Native::Cocoa::core_string_to_ns_string(string) attributes:@{NSParagraphStyleAttributeName:paragraphStyle,NSFontAttributeName:(__bridge id)ct_font->font}]);
    CTFrameRef ctFrame = CTFramesetterCreateFrame(framesetter,CFRangeMake(0,string.size()),CGPathCreateWithRect(CGRectMake(ftextRect.pos.x,ftextRect.pos.y, ftextRect.dimen.minWidth, ftextRect.dimen.minHeight),NULL),NULL);
    
    typedef unsigned char Byte;
    void *data = new Byte[ftextRect.dimen.minWidth * ftextRect.dimen.minHeight * 4];
    /// Setup CGContext!
    CGContextRef context = CGBitmapContextCreate(data,ftextRect.dimen.minWidth,ftextRect.dimen.minHeight,8,ftextRect.dimen.minWidth * 4,CGColorSpaceCreateDeviceRGB(),kCGImageAlphaPremultipliedLast);
    CGContextSetTextMatrix(context,CGAffineTransformIdentity);
    CGContextTranslateCTM(context,0,ftextRect.dimen.minHeight);
    CGContextScaleCTM(context,1.0,-1.0);
    CGContextSetInterpolationQuality(context,kCGInterpolationHigh);
    CGContextSetAllowsAntialiasing(context,true);
    CGContextSetAllowsFontSmoothing(context,true);
    CGContextSetAllowsFontSubpixelPositioning(context,true);
    CGContextSetAllowsFontSubpixelQuantization(context,true);
    CGContextSetShouldAntialias(context,true);
    CGContextSetShouldSmoothFonts(context,true);
    CGContextSetShouldSubpixelPositionFonts(context,true);
    CGContextSetShouldSubpixelQuantizeFonts(context,true);
    CTFrameDraw(ctFrame,context);
    CGContextRelease(context);
    /// Triangulate Rect for Text!
    
    auto mesh_ptr = triangulator->triangulateToTexturedMesh(ftextRect);
    auto & mesh = *mesh_ptr;
    auto & tri1 = mesh[0];
    auto & tri2 = mesh[1];
    tri2.a.textureCoord = tri1.a.textureCoord = {0.f,1.f};
    tri1.b.textureCoord = {1.f,1.f};
    tri2.b.textureCoord = {0.f,0.f};
    tri2.c.textureCoord = tri1.c.textureCoord = {1.f,0.f};
    /// Buffer ID = Visual Id + 0;
    id<MTLBuffer> v_buffer = textured_mesh_to_mtl_vertex_buffer(mesh_ptr.get(),device->metal_device);
    vertexBuffers.push_back(v_buffer);
    
    MTLTextureDescriptor *desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:ftextRect.dimen.minWidth height:ftextRect.dimen.minHeight mipmapped:NO];
    desc.textureType = MTLTextureType2D;
//    desc.storageMode = MTLStorageModePrivate;
    desc.sampleCount = 1;
    id<MTLTexture> texture = [device->metal_device newTextureWithDescriptor:desc];
    [texture replaceRegion:MTLRegionMake2D(0,0,ftextRect.dimen.minWidth,ftextRect.dimen.minHeight) mipmapLevel:0 withBytes:data bytesPerRow:ftextRect.dimen.minWidth * 4];
    
    delete [] (Byte *) data;
    
    auto _v_id = vertexBuffers.size()-1;
    
    textures.insert(std::make_pair(_v_id,texture));
    
    /// Push Multi Sampled Render Pass to Queue to be committed to the gpu.
    RenderPipeline rp;
    rp.pipelineState = device->texture2DPrimitive;
//    rp.multiSampled = true;
//    rp.sampleCount = 4;
    rp.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Textured Rectangle with Text Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        [encoder setFragmentTexture:textures[v_id] atIndex:OMEGAWTK_METAL_TEXTURE_BUFFER];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
        textures.erase(textures.find(v_id));
    };

    renderPasses.push(std::move(rp));
    
    
};

Core::SharedPtr<BDCompositionImage> MTLBDCompositionRenderTarget::createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> & img, Core::Rect & newSize,unsigned v_id){
    auto imgHeader = img->header.get();
    MTLTextureDescriptor *textureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:computePixelFormat(imgHeader->bitDepth, imgHeader->channels,img->sRGB) width:imgHeader->width height:imgHeader->height mipmapped:NO];
    textureDesc.textureType = MTLTextureType2D;
    id<MTLTexture> texture = [device->metal_device newTextureWithDescriptor:textureDesc];
    [texture replaceRegion:MTLRegionMake2D(0,0,imgHeader->width,imgHeader->height) mipmapLevel:0 withBytes:img->data bytesPerRow:imgHeader->stride];
    
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    auto fnewSize = FRect(float(newSize.pos.x) * scaleFactor,float(newSize.pos.y) * scaleFactor,float(newSize.dimen.minWidth) * scaleFactor,float(newSize.dimen.minHeight) * scaleFactor);

    MPSScaleTransform transform;
    NSLog(@"ScaleX:%f , ScaleY:%f",fnewSize.dimen.minWidth/float(imgHeader->width),fnewSize.dimen.minHeight/float(imgHeader->height));
    transform.scaleX = fnewSize.dimen.minWidth/float(imgHeader->width);
    transform.scaleY = fnewSize.dimen.minHeight/float(imgHeader->height);
    transform.translateX = transform.translateY = double(0);
    MPSImageLanczosScale *scaleFix = [[MPSImageLanczosScale alloc] initWithDevice:device->metal_device];
    scaleFix.scaleTransform = &transform;

    id<MTLCommandBuffer> commandBuffer = device->makeNewMTLCommandBuffer();

    MTLTextureDescriptor *destTextureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:fnewSize.dimen.minWidth height:fnewSize.dimen.minHeight mipmapped:NO];
    destTextureDesc.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
    id<MTLTexture> destTexture = [device->metal_device newTextureWithDescriptor:destTextureDesc];
    id<MTLTexture> fin;

    [scaleFix encodeToCommandBuffer:commandBuffer sourceTexture:texture destinationTexture:destTexture];
//    if(img->hasGamma){
//        id<MTLTexture> finalTexture = [device->metal_device newTextureWithDescriptor:destTextureDesc];
//        CIImage *ci_img = [CIImage imageWithMTLTexture:destTexture options:@{}];
////        CIImage *f_img = [ci_img imageByApplyingFilter:@"CIGammaAdjust" withInputParameters:@{@"inputImage":ci_img,@"inputPower":[NSNumber numberWithDouble:]}];
//        CIContext *ci_context = [CIContext contextWithMTLDevice:device->metal_device];
//        [ci_context render:f_img toMTLTexture:finalTexture commandBuffer:commandBuffer bounds:f_img.extent colorSpace:f_img.colorSpace];
//        fin = finalTexture;
//    }
//    else {
    fin = destTexture;
//    };
    [commandBuffer commit];
//    [commandBuffer waitUntilCompleted];
    commandBuffers.push_back(commandBuffer);

    auto rc = MTLBDCompositionImage::Create(img,fnewSize,destTextureDesc,fin);
    images.insert(std::make_pair(v_id,rc));
    return rc;
};

void MTLBDCompositionRenderTarget::drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos){
    MTLBDCompositionImage *mtlimg = reinterpret_cast<MTLBDCompositionImage *>(img.get());
    
    
    auto rc = triangulator->triangulateToTexturedMesh(mtlimg->rect);
    auto & mesh = *rc;
    auto & tri1 = mesh[0];
    auto & tri2 = mesh[1];
    tri2.a.textureCoord = tri1.a.textureCoord = {0.f,1.f};
    tri1.b.textureCoord = {1.f,1.f};
    tri2.b.textureCoord = {0.f,0.f};
    tri2.c.textureCoord = tri1.c.textureCoord = {1.f,0.f};
    id<MTLBuffer> vertexBuffer = textured_mesh_to_mtl_vertex_buffer(rc.get(),device->metal_device);
    vertexBuffers.push_back(vertexBuffer);
    auto _v_id = vertexBuffers.size()-1;

    textures.insert(std::make_pair(_v_id,mtlimg->img));

    RenderPipeline rp;
    rp.pipelineState = device->texture2DPrimitive;
    rp.setupCallback = [=](id<MTLRenderCommandEncoder> encoder,unsigned v_id){
        [encoder setLabel:@"Textured Rectangle Pipeline"];
        [encoder setVertexBuffer:vertexBuffers[v_id] offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
        [encoder setFragmentTexture:textures[v_id] atIndex:OMEGAWTK_METAL_TEXTURE_BUFFER];
        [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
        textures.erase(textures.find(v_id));
    };

    renderPasses.push(std::move(rp));
};

void MTLBDCompositionRenderTarget::commit(){
//    NSLog(@"Commit!!!");
//        NSLog(@"Commit!!!");
    
    float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
    @autoreleasepool {
        
        for(auto & buffer : commandBuffers){
            [buffer waitUntilCompleted];
        };
        commandBuffers.clear();
    
    
//
        id<CAMetalDrawable> drawable = [metalLayer nextDrawable];
        
        id<MTLCommandBuffer> finalCommandBuffer = device->makeNewMTLCommandBuffer();
        /// Clear the Screen!
        {
            NSColor *nscolor = color_to_ns_color(clearColor);
            NSLog(@"Clear Color %@",nscolor);
            MTLRenderPassDescriptor *initialRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
            initialRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
            initialRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
            initialRenderPassDesc.renderTargetWidth = float(native_item->rect.dimen.minWidth) * scaleFactor;
            initialRenderPassDesc.renderTargetHeight = float(native_item->rect.dimen.minHeight) * scaleFactor;
            initialRenderPassDesc.renderTargetArrayLength = 1;
            initialRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
//            initialRenderPassDesc.defaultRasterSampleCount = 0;
            initialRenderPassDesc.colorAttachments[0].texture = drawable.texture;
            
            MTLRenderPassDescriptor *mainRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
            mainRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
            mainRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
            mainRenderPassDesc.renderTargetWidth = float(native_item->rect.dimen.minWidth) * scaleFactor;
            mainRenderPassDesc.renderTargetHeight = float(native_item->rect.dimen.minHeight) * scaleFactor;
            mainRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStore;
            mainRenderPassDesc.renderTargetArrayLength = 1;
            mainRenderPassDesc.defaultRasterSampleCount = 0;
            mainRenderPassDesc.colorAttachments[0].texture = drawable.texture;
            
            MTLRenderPassDescriptor *multiSampledRenderPassDesc = [MTLRenderPassDescriptor renderPassDescriptor];
            multiSampledRenderPassDesc.colorAttachments[0].loadAction = MTLLoadActionLoad;
            multiSampledRenderPassDesc.colorAttachments[0].clearColor = MTLClearColorMake(nscolor.redComponent,nscolor.greenComponent,nscolor.blueComponent,nscolor.alphaComponent);
            multiSampledRenderPassDesc.colorAttachments[0].resolveTexture = drawable.texture;
            multiSampledRenderPassDesc.renderTargetWidth = float(native_item->rect.dimen.minWidth) * scaleFactor;
            multiSampledRenderPassDesc.renderTargetHeight = float(native_item->rect.dimen.minHeight) * scaleFactor;
            multiSampledRenderPassDesc.colorAttachments[0].storeAction = MTLStoreActionStoreAndMultisampleResolve;
            multiSampledRenderPassDesc.renderTargetArrayLength = 1;
            
            MTLTextureDescriptor *multiSampleTextureDesc = [[MTLTextureDescriptor alloc] init];
            multiSampleTextureDesc.textureType = MTLTextureType2DMultisample;
            multiSampleTextureDesc.storageMode = MTLStorageModePrivate;
            multiSampleTextureDesc.width = float(native_item->rect.dimen.minWidth) * scaleFactor;
            multiSampleTextureDesc.height = float(native_item->rect.dimen.minHeight) * scaleFactor;
            multiSampleTextureDesc.pixelFormat = MTLPixelFormatBGRA8Unorm;
            multiSampleTextureDesc.usage = MTLTextureUsageRenderTarget;
            
            id<MTLRenderCommandEncoder> initialRenderPass = [finalCommandBuffer renderCommandEncoderWithDescriptor:initialRenderPassDesc];
            [initialRenderPass endEncoding];
            unsigned idx = 0;
            while(!renderPasses.empty()){
                auto renderPass = renderPasses.front();
                renderPasses.pop();
                id<MTLRenderCommandEncoder> rp;
                if(renderPass.multiSampled){
                    multiSampleTextureDesc.sampleCount = renderPass.sampleCount;
                    id<MTLTexture> msTexture = [device->metal_device newTextureWithDescriptor:multiSampleTextureDesc];
                    multiSampledRenderPassDesc.colorAttachments[0].texture = msTexture;
                    rp = [finalCommandBuffer renderCommandEncoderWithDescriptor:multiSampledRenderPassDesc];
                }
                else {
                    rp = [finalCommandBuffer renderCommandEncoderWithDescriptor:mainRenderPassDesc];
                };
                [rp setRenderPipelineState:renderPass.pipelineState];
                renderPass.setupCallback(rp,idx);
                [rp endEncoding];
                ++idx;
            };
        }
        
        [finalCommandBuffer presentDrawable:drawable];
        [finalCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer){
                auto buffer_it = vertexBuffers.begin();
                while(buffer_it != vertexBuffers.end()){
                    id<MTLBuffer> buffer = *buffer_it;
                    [buffer setPurgeableState:MTLPurgeableStateEmpty];
                    ++buffer_it;
                };
                vertexBuffers.clear();
        }];
        [finalCommandBuffer commit];
        [metalLayer setNeedsDisplay];
    };
//    [drawable release];
};

};
