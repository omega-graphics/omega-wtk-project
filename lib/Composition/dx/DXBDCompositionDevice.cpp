#include "DXBDCompositionDevice.h"
#include "DXBDCompositionRenderTarget.h"
#include "DXBDCompositionFontFactory.h"
#include "DXBDCompositionImage.h"

namespace OmegaWTK::Composition {
    DXBDCompositionDevice::DXBDCompositionDevice(){
        HRESULT hr;
        D3D_FEATURE_LEVEL feature_level[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };
        hr = D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,D3D11_CREATE_DEVICE_BGRA_SUPPORT,feature_level,ARRAYSIZE(feature_level),D3D11_SDK_VERSION,&direct3d_device, NULL, NULL);
        if(FAILED(hr)){

        };
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,&direct2d_factory);
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = direct3d_device->QueryInterface(IID_PPV_ARGS(&dxgi_device));
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = dxgi_device->GetAdapter(&dxgi_adapter);
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = dxgi_device->SetMaximumFrameLatency(3);
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = direct2d_factory->CreateDevice(dxgi_device.get(),&direct2d_device);
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = DCompositionCreateDevice3(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device));
        if(FAILED(hr)){
            //Handle Error!
        };

    };
    DXBDCompositionDevice::~DXBDCompositionDevice(){
        Core::SafeRelease(&dcomp_device);
        Core::SafeRelease(&direct2d_device);
        Core::SafeRelease(&dxgi_factory);
        Core::SafeRelease(&dxgi_adapter);
        Core::SafeRelease(&direct2d_factory);
        Core::SafeRelease(&direct3d_device);
    };

    Core::SharedPtr<BDCompositionDevice> DXBDCompositionDevice::Create(){
        return std::make_shared<DXBDCompositionDevice>();
    };
    Core::SharedPtr<BDCompositionLayerRenderTarget> DXBDCompositionDevice::makeLayerRenderTarget(Layer *layer){
        // MessageBoxA(GetForegroundWindow(),"Making DX Render Target","",MB_OK);
        return DXBDCompositionLayerRenderTarget::Create(this,(Native::Win::HWNDItem *)layer->getTargetNativePtr());
    };
    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionDevice::makeImageRenderTarget(Core::Rect &size){
        UINT dpi = GetDpiForWindow(GetForegroundWindow());
        ID2D1DeviceContext *device_context;
        ID2D1Bitmap *bitmap;
        HRESULT hr;
        hr = direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&device_context);
        if(FAILED(hr)){

        };
        hr = device_context->CreateBitmap(D2D1::SizeU(size.dimen.minWidth,size.dimen.minHeight),D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&bitmap);
        if(FAILED(hr)){

        };
        return DXBDCompositionImageRenderTarget::Create(this,size,bitmap,device_context);
    };
    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionDevice::makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img){
        DXBDCompositionImage *dxImg = (DXBDCompositionImage *)img.get();
        ID2D1DeviceContext *device_context;
        HRESULT hr;
        hr = direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&device_context);
        if(FAILED(hr)){

        };
        return DXBDCompositionImageRenderTarget::Create(this,dxImg->rect,dxImg->native_image.get(),device_context);
    };
    Core::SharedPtr<BDCompositionFontFactory> DXBDCompositionDevice::createFontFactory(){
        HRESULT hr;
        IDWriteFactory *factory;
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),(IUnknown **)&factory);
        if(FAILED(hr)){

        };
        return DXBDCompositionFontFactory::Create(factory);
    };
    void DXBDCompositionDevice::destroyTarget(Layer *layer, Core::SharedPtr<BDCompositionRenderTarget> &target){

    };
};