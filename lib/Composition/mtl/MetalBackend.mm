#import "MetalBackend.h"
#include "MetalBridgeHeader.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import <MetalPerformanceShaders/MetalPerformanceShaders.h>

namespace OmegaWTK::Composition {

MTLPixelFormat computePixelFormat(Core::BitmapImage::ColorFormat & color,Core::BitmapImage::AlphaFormat & alpha,int bitDepth,bool issRGB){
    switch (color) {
        case Core::BitmapImage::ColorFormat::RGBA  :{
            if(bitDepth == 8){
                if(issRGB) {
//                    NSLog(@"Yes");
                    return MTLPixelFormatRGBA8Unorm_sRGB;
                }
                else
                    return MTLPixelFormatRGBA8Unorm;
            }
            else if(bitDepth == 16){
                return MTLPixelFormatRGBA16Unorm;
            };
            break;
        }
        case Core::BitmapImage::ColorFormat::RGB :{
            break;
        }
        default:
            break;
    }
};

class MetalBackend {
public:
    id<MTLDevice> device;
    id<MTLLibrary> library;
    id<MTLCommandQueue> commandQueue;
    
    typedef id<MTLTexture> Texture;
    typedef id<MTLRenderPipelineState> RenderPipelineState;
    typedef id<MTLBuffer> Buffer;
    
    typedef enum : OPT_PARAM {
        Rectangle
    } Object;
    
    RenderPipelineState bitmapRenderPipelineState;
    Core::Map<unsigned,Texture> metalTextures;
    Core::Map<unsigned,Buffer> metalVertexBuffers;
    
    
    void setupBitmapRenderPipeline(){
        MTLRenderPipelineDescriptor *descriptor = [[MTLRenderPipelineDescriptor alloc] init];
        descriptor.vertexFunction = [library newFunctionWithName:@"solidRectVertex"];
        descriptor.fragmentFunction = [library newFunctionWithName:@"solidRectFragment"];
        descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
//        descriptor.rasterSampleCount = 2;
        NSError *error;
        RenderPipelineState pipelineState = [device newRenderPipelineStateWithDescriptor:descriptor error:&error];
        if(error.code >= 0){
            bitmapRenderPipelineState = pipelineState;
        }
        else {
            NSLog(@"Failed to Create Pipeline State. ERROR:%@",error);
        };
    };
    
    void setupMetalTexture(unsigned v_id,Core::BitmapImage & img){
        auto found = metalTextures.find(v_id);
        if(found == metalTextures.end()){
//            if(img.bitDepth == 16){
//                textureDescriptor.pixelFormat = MTLPixelFormatRGBA16Unorm;
//            };
//            textureDescriptor.mipmapLevelCount = 1;
////            textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
//
//            textureDescriptor.height = img.height;
//            textureDescriptor.width = img.width;
            MTLPixelFormat format = computePixelFormat(img.header->color_format,img.header->alpha_format,img.header->bitDepth,img.sRGB);
            MTLTextureDescriptor *textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:format width:img.header->width height:img.header->height mipmapped:NO];
            textureDescriptor.textureType = MTLTextureType2D;
//            textureDescriptor.sampleCount = 2;
//            textureDescriptor.storageMode = MTLStorageModePrivate;
            Texture texture = [device newTextureWithDescriptor:textureDescriptor];
            metalTextures.insert(std::pair<unsigned,Texture>(v_id,texture));
        };
    };
    
    constexpr const static OmegaWTKMetalVertex ViewSizeRect[] = {
        /// Triangle 1
        { {  1.0, -1.0} , {1.f,0.f}},
        { {  1.0,  1.0} , {1.f,1.f}},
        { { -1.0,  1.0} , {0.f,1.f}},
        
        /// Triangle 2
        { {  1.0, -1.0} , {1.f,0.f}},
        { { -1.0, -1.0} , {0.f,0.f}},
        { { -1.0,  1.0} , {0.f,1.f}}
    };
    
    void setupMetalVertexBuffer(unsigned v_id,Object object_ty){
        auto found = metalVertexBuffers.find(v_id);
        if(found == metalVertexBuffers.end()){
            MTLResourceOptions opts = MTLResourceStorageModeShared;
            if(object_ty == Rectangle){
                Buffer buffer = [device newBufferWithBytes:ViewSizeRect length:sizeof(OmegaWTKMetalVertex) * 6 options:opts];
                metalVertexBuffers.insert(std::pair<unsigned,Buffer>(v_id,buffer));
            };
        };
    };
    
    
    MetalBackend(){
        device = MTLCreateSystemDefaultDevice();
        commandQueue = [device newCommandQueue];
        library = [device newDefaultLibrary];
        setupBitmapRenderPipeline();
    };
};

void *make_metal_backend(){
    return new MetalBackend();
};

CAMetalLayer *drawBitmapToMetalLayer(Core::SharedPtr<Core::BitmapImage> & img,Core::Rect & rect,void *metal_backend,unsigned v_id){
    MetalBackend *backend = (MetalBackend *)metal_backend;
    CAMetalLayer *metalLayer = [CAMetalLayer layer];
//    metalLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
    NSColor *color = [NSColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
    simd_float4 metalBackgroundColor = {float(color.redComponent),float(color.greenComponent),float(color.blueComponent),float(color.alphaComponent)};
    
    if(!img){
        return metalLayer;
    };
    
    [metalLayer setDrawableSize:CGSizeMake(rect.dimen.minWidth,rect.dimen.minHeight)];
    metalLayer.frame = Native::Cocoa::core_rect_to_cg_rect(rect);
//    auto size = [metalLayer drawableSize];
    
    id<MTLDevice> device = backend->device;
    metalLayer.device = device;
    id<CAMetalDrawable> drawable = [metalLayer nextDrawable];
    
    
    id<MTLCommandBuffer> commandBuffer_0 = [backend->commandQueue commandBuffer];
    id<MTLCommandBuffer> commandBuffer_1 = [backend->commandQueue commandBuffer];
    
    MTLRegion region = {
        { 0, 0, 0 },                   // MTLOrigin
        {img->header->width, img->header->height, 1} // MTLSize
    };
    
    MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    if(renderPassDescriptor != nil){
        renderPassDescriptor.renderTargetWidth = rect.dimen.minWidth;
        renderPassDescriptor.renderTargetHeight = rect.dimen.minHeight;
        renderPassDescriptor.renderTargetArrayLength = 1;
//        renderPassDescriptor.defaultRasterSampleCount = 0;
        renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(1.0,1.0,1.0,1.0);
//        renderPassDescriptor.defaultRasterSampleCount = 2;
        
        backend->setupMetalTexture(v_id,*img);
        MetalBackend::Texture texture = backend->metalTextures[v_id];
//        NSUInteger bytesPerRow = ((img.bitDepth * img.channelCount)/8) * img.width;
        
        [texture replaceRegion:region mipmapLevel:0 withBytes:img->data bytesPerRow:img->header->stride];
        backend->setupMetalVertexBuffer(v_id,MetalBackend::Rectangle);
        MetalBackend::Buffer vertexBuffer = backend->metalVertexBuffers[v_id];
        
        auto scalTextureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:rect.dimen.minWidth height:rect.dimen.minHeight mipmapped:NO];
        scalTextureDesc.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite;
        id<MTLTexture> scaleTexture = [device newTextureWithDescriptor:scalTextureDesc];
        /// Scale Down!
        MPSImageLanczosScale *scale = [[MPSImageLanczosScale alloc] initWithDevice:device];
        MPSScaleTransform transform;
        transform.scaleX = double(rect.dimen.minWidth) / double(img->header->width);
        transform.scaleY = double(rect.dimen.minHeight) / double(img->header->height);
        transform.translateX = double(0);
        transform.translateY = double(0);
        scale.scaleTransform = &transform;
        [scale encodeToCommandBuffer:commandBuffer_0 sourceTexture:texture destinationTexture:scaleTexture];
        
        id<MTLTexture> finalTexture;
//            changedDescriptor.mipmapLevelCount = 4;
        {
            MTLTextureDescriptor *changedDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm width:scaleTexture.width height:scaleTexture.height mipmapped:YES];
            changedDescriptor.usage = MTLTextureUsageShaderWrite | MTLTextureUsageShaderRead;
            id<MTLTexture> fin = [device newTextureWithDescriptor:changedDescriptor];
            CIImage *image = [CIImage imageWithMTLTexture:scaleTexture options:@{}];
//            image = [image imageBySamplingLinear];
            if(img->hasGamma){
                image = [image imageByApplyingFilter:@"CIGammaAdjust" withInputParameters:@{@"inputImage":image,@"inputPower":[NSNumber numberWithFloat:img->gamma]}];
            }
            CIContext *context = [CIContext contextWithMTLDevice:device];
            [context render:image toMTLTexture:fin commandBuffer:commandBuffer_0 bounds:[image extent] colorSpace:[image colorSpace]];
            finalTexture = fin;
        }
//        id<MTLBlitCommandEncoder> mipmapgenerator = [commandBuffer_0 blitCommandEncoder];
//        [mipmapgenerator generateMipmapsForTexture:finalTexture];
//        [mipmapgenerator endEncoding];
        [commandBuffer_0 addCompletedHandler:^(id<MTLCommandBuffer> buffer){
            id<MTLRenderCommandEncoder> encoder = [commandBuffer_1 renderCommandEncoderWithDescriptor:renderPassDescriptor];
            [encoder setRenderPipelineState:backend->bitmapRenderPipelineState];
            [encoder setVertexBuffer:vertexBuffer offset:0 atIndex:OMEGAWTK_METAL_VERICES_BUFFER];
            [encoder setFragmentBytes:&metalBackgroundColor length:sizeof(metalBackgroundColor) atIndex:OMEGAWTK_METAL_BACKGROUND_COLOR_BUFFER];
            [encoder setFragmentTexture:finalTexture atIndex:OMEGAWTK_METAL_TEXTURE_BUFFER];
            [encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
            [encoder endEncoding];
            [commandBuffer_1 presentDrawable:drawable];
            [commandBuffer_1 commit];
        }];
        [commandBuffer_0 commit];
        

        
        
    };
    [metalLayer setNeedsDisplay];
    
    return metalLayer;
};

};
