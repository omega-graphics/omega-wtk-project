#include "DXBDCompositionImage.h"
#include "DXBDCompositionRenderTarget.h"

namespace OmegaWTK::Composition {
DXBDCompositionImage::DXBDCompositionImage(Core::SharedPtr<Core::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionRenderTarget *renderTarget,ID2D1Image *img):BDCompositionImage(_img),renderTarget(renderTarget),rect(rect),native_image(img){

};

Core::SharedPtr<BDCompositionImage> DXBDCompositionImage::Create(Core::SharedPtr<Core::BitmapImage> & _img,Core::Rect & rect,DXBDCompositionRenderTarget *renderTarget,ID2D1Image *img){
    return std::make_shared<DXBDCompositionImage>(_img,rect,renderTarget,img);
};

void DXBDCompositionImage::redoImage(){

    HRESULT hr;

    Core::SafeRelease(&native_image);

    // bitmapImage = newImg;

    auto & header = bitmapImage->header;
    UINT dpi = GetDpiForWindow(renderTarget->hwndItem->getHandle());
    FLOAT scaleFactor = FLOAT(dpi)/96.f;
    ID2D1Bitmap *bitmap;
    ID2D1Image *res, *res2;
    hr = renderTarget->direct2d_device_context->CreateBitmap(D2D1::SizeU(header->width,header->height),bitmapImage->data,header->stride,D2D1::BitmapProperties(D2D1::PixelFormat(computePixelFormat(header->bitDepth,header->channels,bitmapImage->sRGB),D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&bitmap);
    if(FAILED(hr)){
    //     MessageBoxA(GetForegroundWindow(),"Failed to Create Bitmap!","NOTE",MB_OK);
    }
    // else {
    //     MessageBoxA(GetForegroundWindow(),"Successfuly Created Bitmap!","NOTE",MB_OK);
    // }
    ID2D1Effect *alphaFix, *scaleFix;
    hr = renderTarget->direct2d_device_context->CreateEffect(CLSID_D2D1Premultiply,&alphaFix);
    if(FAILED(hr)){

    };
    hr = renderTarget->direct2d_device_context->CreateEffect(CLSID_D2D1Scale,&scaleFix);
    if(FAILED(hr)){

    };
    scaleFix->SetValue(D2D1_SCALE_PROP_INTERPOLATION_MODE,D2D1_SCALE_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR);
    scaleFix->SetValue(D2D1_SCALE_PROP_SCALE,D2D1::Vector2F(FLOAT(rect.dimen.minWidth * scaleFactor)/FLOAT(header->width),FLOAT(rect.dimen.minHeight * scaleFactor)/FLOAT(header->height)));

    scaleFix->SetInput(0,bitmap);
    scaleFix->GetOutput(&res);
    alphaFix->SetInput(0,res);
    alphaFix->GetOutput(&res2);

    native_image = res2;
};

DXGI_FORMAT computePixelFormat(unsigned bitDepth,unsigned channelCount,bool isrgb){
            // std::string message = "BitDepth :" + std::to_string(bitDepth) + "ChannelCount:" + std::to_string(channelCount);
            // MessageBox(GetForegroundWindow(),message.c_str(),"NOTE",MB_OK);
            if(bitDepth == 32){
                switch (channelCount) {
                    case 1 : {
                        return DXGI_FORMAT_R32_TYPELESS;
                         break;
                    };
                    case 2 : {
                        return DXGI_FORMAT_R32G32_TYPELESS;
                         break;
                    };
                    case 3 : {
                        return DXGI_FORMAT_R32G32B32_TYPELESS;
                         break;
                    };
                    case 4 : {
                        return  DXGI_FORMAT_R32G32B32A32_TYPELESS;
                         break;
                    };
                }
            }
            else if(bitDepth == 16){
                switch (channelCount) {
                    case 1 : {
                        return DXGI_FORMAT_R16_TYPELESS;
                         break;
                    };
                    case 2 : {
                        return DXGI_FORMAT_R16G16_TYPELESS;
                         break;
                    };
                    // case 3 : {
                    //     return DXGI_FORMAT_R16G16B16_T;
                    // };
                    case 4 : {
                        return  DXGI_FORMAT_R16G16B16A16_TYPELESS;
                         break;
                    };
                }
            }
            else if(bitDepth == 8){
                switch (channelCount) {
                    case 1 : {
                        return DXGI_FORMAT_R8_TYPELESS;
                         break;
                    };
                    case 2 : {
                        return DXGI_FORMAT_R8G8_TYPELESS;
                         break;
                    };
                    // case 3 : {
                    //     return DXGI_FORMAT_R8G8B8_TYPELESS;
                    // };
                    case 4 : {
                        if(isrgb)
                            return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
                        else
                            return  DXGI_FORMAT_R8G8B8A8_UNORM;
                        break;
                    };
                }
            };
            
        };
}