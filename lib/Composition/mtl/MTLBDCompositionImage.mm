#import "MTLBDCompositionImage.h"
#import "MTLBDCompositionRenderTarget.h"

#import <CoreImage/CoreImage.h>

namespace OmegaWTK::Composition {
MTLBDCompositionImage::MTLBDCompositionImage(MTLBDCompositionDevice *device,Core::SharedPtr<Media::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img):BDCompositionImage(_img),device(device),n_rect(Rect(rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight)),rect(rect),desc(desc),img(img){
    
};

MTLBDCompositionImage::MTLBDCompositionImage(MTLBDCompositionDevice *device,Core::Rect &rect,id<MTLTexture> img,MTLTextureDescriptor *desc):BDCompositionImage(),device(device),n_rect(rect),rect(FRect(rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight)),desc(desc),img(img){
    
};

Core::SharedPtr<BDCompositionImage> MTLBDCompositionImage::Create(MTLBDCompositionDevice *device,Core::SharedPtr<Media::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img){
    return std::make_shared<MTLBDCompositionImage>(device,_img,rect,desc,img);
};

Core::SharedPtr<BDCompositionImage> MTLBDCompositionImage::Create(MTLBDCompositionDevice *device,Core::Rect & rect, id<MTLTexture> img,MTLTextureDescriptor *desc){
    return std::make_shared<MTLBDCompositionImage>(device,rect,img,desc);
};

void MTLBDCompositionImage::applyEffect(LayerEffect *effect){
    ///Effects
    switch (effect->type) {
        case LayerEffect::DropShadow :{
            dropShadow = effect;
            break;
        }
//        case LayerEffect::Transformation : {
//            VisualEffect::TransformationParams *params = (VisualEffect::TransformationParams *)effect.params;
//
//            break;
//        };
        default:
            imageEffects.push_back(effect);
            break;
    }
};

void MTLBDCompositionImage::commitEffects(){
    id<MTLTexture> replacementTexture = [device->metal_device newTextureWithDescriptor:desc];
    CIImage *ciImg = [CIImage imageWithMTLTexture:img options:@{}];
    auto effect_it = imageEffects.begin();
    while (effect_it != imageEffects.end()) {
        auto _effect = *effect_it;
        switch (_effect->type) {
            case LayerEffect::GaussianBlur:
            {
                LayerEffect::GaussianBlurParams *params = (LayerEffect::GaussianBlurParams *)_effect->params;
                ciImg = [ciImg imageByApplyingFilter:@"CIGaussianBlur" withInputParameters:@{@"inputImage":ciImg,@"inputRadius":[NSNumber numberWithFloat:params->radius]}];
                break;
            }
            case LayerEffect::DirectionalBlur : {
                LayerEffect::DirectionalBlurParams *params = (LayerEffect::DirectionalBlurParams *)_effect->params;
                ciImg = [ciImg imageByApplyingFilter:@"CIMotionBlur" withInputParameters:@{@"inputImage":ciImg,@"inputRadius":[NSNumber numberWithFloat:params->radius],@"inputAngle":[NSNumber numberWithFloat:params->angle]}];
                break;
            }
            default:
                break;
        }
        ++effect_it;
    }
    CIContext *ciContext = [CIContext contextWithMTLDevice:device->metal_device];
    id<MTLCommandBuffer> buffer = device->makeNewMTLCommandBuffer();
    [ciContext render:ciImg toMTLTexture:replacementTexture commandBuffer:buffer bounds:[ciImg extent] colorSpace:CGColorSpaceCreateDeviceRGB()];
    [buffer commit];
    [buffer waitUntilCompleted];
    [img release];
    img = replacementTexture;
};

MTLPixelFormat computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb){
    switch (bitDepth) {
        case 8: {
            if(channelCount == 4){
                if(isrgb)
                    return MTLPixelFormatRGBA8Unorm_sRGB;
                else
                    return MTLPixelFormatRGBA8Unorm;
            };
            break;
        }
        case 16 : {
            if(channelCount == 4){
                return MTLPixelFormatRGBA16Unorm;
            };
            break;
        }
        default:
            return MTLPixelFormatInvalid;
            break;
    }
};

};
