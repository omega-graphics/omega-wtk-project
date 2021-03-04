#include "../BDCompositionRenderTarget.h"
#include "MTLBDCompositionDevice.h"
#include "MTLBDTriangulator.h"
#include "MTLBDCompositionFontFactory.h"
#include "MTLBDCompositionImage.h"

#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import <QuartzCore/QuartzCore.h>
#import <MetalPerformanceShaders/MetalPerformanceShaders.h>
#import <CoreImage/CoreImage.h>

#include <functional>

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONRENDERTARGET_H



namespace OmegaWTK::Composition {
    template<class _Parent>
    class MTLBDCompositionRenderTarget : public _Parent {
        Core::UniquePtr<MTLBDTriangulator> triangulator;
    protected:
        MTLBDCompositionDevice *device;
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
        MTLBDCompositionRenderTarget(MTLBDCompositionDevice *device,Color init_clear_color,Core::Rect &target_frame);
        virtual void clear(Color &clear_color);
        void frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned width);
        void frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned width);
        void fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush);
        void fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush);
        void drawText(Core::SharedPtr<BDCompositionFont> &font, Core::String &string, Core::Rect &textRect, Core::SharedPtr<Brush> &brush);
        Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> &img, Core::Rect &newSize, unsigned v_id);
        void drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos);
    };

NSColor * color_to_ns_color(Color & color);

id<MTLBuffer> solid_color_2d_mesh_to_mtl_vertex_buffer(MTLBDTriangulator::SolidColor2DMesh * mesh_ptr,id<MTLDevice> device);
id<MTLBuffer> textured_mesh_to_mtl_vertex_buffer(MTLBDTriangulator::Textured2DMesh * mesh_ptr,id<MTLDevice> device);

template<class _Ty>
MTLBDCompositionRenderTarget<_Ty>::MTLBDCompositionRenderTarget(MTLBDCompositionDevice *device,Color init_clear_color,Core::Rect &target_frame):device(device),clearColor(init_clear_color),triangulator(std::make_unique<MTLBDTriangulator>(target_frame)){
    
};

template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::clear(Color & clear_color){
//    NSLog(@"OmegaWTK Color: R %i G %i B %i A %i",clear_color.r,clear_color.g,clear_color.b,clear_color.a);
    clearColor = std::move(clear_color);
};
template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::fillRect(Core::Rect &rect,Core::SharedPtr<Brush> & brush){
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
template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::frameRect(Core::Rect &rect,Core::SharedPtr<Brush> & brush,unsigned width){
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
template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::fillRoundedRect(Core::RoundedRect & rect, Core::SharedPtr<Brush> & brush){
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
template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::frameRoundedRect(Core::RoundedRect & rect, Core::SharedPtr<Brush> & brush, unsigned width){
    /// TODO: Implement!
};
template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::drawText(Core::SharedPtr<BDCompositionFont> & font, Core::String & string, Core::Rect &textRect, Core::SharedPtr<Brush> & brush){
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
//    CGContextSetInterpolationQuality(context,kCGInterpolationHigh);
//    CGContextSetAllowsAntialiasing(context,true);
//    CGContextSetAllowsFontSmoothing(context,true);
//    CGContextSetAllowsFontSubpixelPositioning(context,true);
//    CGContextSetAllowsFontSubpixelQuantization(context,true);
//    CGContextSetShouldAntialias(context,true);
//    CGContextSetShouldSmoothFonts(context,true);
//    CGContextSetShouldSubpixelPositionFonts(context,true);
//    CGContextSetShouldSubpixelQuantizeFonts(context,true);
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
template<class _Ty>
Core::SharedPtr<BDCompositionImage> MTLBDCompositionRenderTarget<_Ty>::createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> & img, Core::Rect & newSize,unsigned v_id){
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
    this->images.insert(std::make_pair(v_id,rc));
    return rc;
};
template<class _Ty>
void MTLBDCompositionRenderTarget<_Ty>::drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos){
    MTLBDCompositionImage *mtlimg = reinterpret_cast<MTLBDCompositionImage *>(img.get());
    auto scaledRect = mtlimg->rect;
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
//    scaledRect.pos.x *= scaleFactor;
//    scaledRect.pos.y *= scaleFactor;
//    scaledRect.dimen.minWidth *= scaleFactor;
//    scaledRect.dimen.minHeight *= scaleFactor;
    
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






class MTLBDCompositionViewRenderTarget : public MTLBDCompositionRenderTarget<BDCompositionViewRenderTarget> {
    CAMetalLayer *metalLayer;
    Native::Cocoa::CocoaItem *native_item;
    id<CAMetalDrawable> currentDrawable = nil;
    friend class MTLBDCALayerTree;
    Core::Rect & rect;
#ifdef TARGET_MACOS
//    CVDisplayLink *displayLink;
#endif
public:
    void clear(Color &clear_color);
    MTLBDCompositionViewRenderTarget(MTLBDCompositionDevice *device,Core::Rect & rect);
    static Core::SharedPtr<MTLBDCompositionViewRenderTarget> Create(MTLBDCompositionDevice *device,Core::Rect & rect);
    void commit();
};

class MTLBDCompositionImageRenderTarget : public MTLBDCompositionRenderTarget<BDCompositionImageRenderTarget> {
    id<MTLTexture> target;
    Core::Rect rect;
public:
    MTLBDCompositionImageRenderTarget(MTLBDCompositionDevice *device,Core::Rect & rect,id<MTLTexture> target);
    static Core::SharedPtr<BDCompositionImageRenderTarget> Create(MTLBDCompositionDevice *device,Core::Rect & rect,id<MTLTexture> target);
    void commit();
    Core::SharedPtr<BDCompositionImage> getImg();
};

};

#endif
