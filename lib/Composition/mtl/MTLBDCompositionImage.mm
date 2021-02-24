#import "MTLBDCompositionImage.h"

namespace OmegaWTK::Composition {
MTLBDCompositionImage::MTLBDCompositionImage(Core::SharedPtr<Core::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img):BDCompositionImage(_img),rect(rect),desc(desc),img(img){
    
};

Core::SharedPtr<BDCompositionImage> MTLBDCompositionImage::Create(Core::SharedPtr<Core::BitmapImage> & _img,Core::FRect & rect,MTLTextureDescriptor *desc,id<MTLTexture> img){
    return std::make_shared<MTLBDCompositionImage>(_img,rect,desc,img);
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
