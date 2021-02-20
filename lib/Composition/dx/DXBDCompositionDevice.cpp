#include "DXBDCompositionDevice.h"
#include "DXBDCompositionRenderTarget.h"
#include "DXBDCompositionFontFactory.h"

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
        hr = dxgi_device->SetMaximumFrameLatency(1);
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
    Core::SharedPtr<BDCompositionRenderTarget> DXBDCompositionDevice::makeTarget(Layer *layer){
        // MessageBoxA(GetForegroundWindow(),"Making DX Render Target","",MB_OK);
        return DXBDCompositionRenderTarget::Create(this,(Native::Win::HWNDItem *)layer->getTargetNativePtr());
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