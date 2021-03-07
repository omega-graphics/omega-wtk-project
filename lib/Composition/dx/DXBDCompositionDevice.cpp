#include "DXBDCompositionDevice.h"
#include "DXBDCompositionRenderTarget.h"
#include "DXBDCompositionFontFactory.h"
#include "DXBDCompositionImage.h"
#include "DCVisualTree.h"

namespace OmegaWTK::Composition {
    DXBDCompositionDevice::DXBDCompositionDevice(){
        HRESULT hr;
        D3D_FEATURE_LEVEL feature_level[] = {
            #ifdef DIRECT3D_12
                D3D_FEATURE_LEVEL_12_0,
            #endif
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };
        #ifdef DIRECT3D_12

        hr = CreateDXGIFactory2(0,IID_PPV_ARGS(&dxgi_factory));
        if(FAILED(hr)){

        };

        hr = D3D12CreateDevice(NULL,D3D_FEATURE_LEVEL_12_0,IID_PPV_ARGS(&direct3d_device));
        if(FAILED(hr)){

        };

        hr = dxgi_factory->EnumAdapterByLuid(direct3d_device->GetAdapterLuid(),IID_PPV_ARGS(&dxgi_adapter));
        if(FAILED(hr)){

        };
        // hr = direct3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&direct_3d_command_allocator));
        // if(FAILED(hr)){

        // };
        D3D12_COMMAND_QUEUE_DESC queue_desc;
        queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
        queue_desc.NodeMask = direct3d_device->GetNodeCount();

        hr = direct3d_device->CreateCommandQueue(&queue_desc,IID_PPV_ARGS(&direct3d_command_queue));
        if(FAILED(hr)){

        };

        hr = D3D11On12CreateDevice(direct3d_device.get(),D3D11_CREATE_DEVICE_BGRA_SUPPORT,feature_level,ARRAYSIZE(feature_level),(IUnknown *const*)&direct3d_command_queue,1,direct3d_device->GetNodeCount(),&direct3d_device_11_priv,NULL,NULL);
        if(FAILED(hr)){

        };
        #else
        hr = D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,D3D11_CREATE_DEVICE_BGRA_SUPPORT,feature_level,ARRAYSIZE(feature_level),D3D11_SDK_VERSION,&direct3d_device, NULL, NULL);
        if(FAILED(hr)){

        };
        #endif
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,&direct2d_factory);
        if(FAILED(hr)){
            //Handle Error!
        };

        #ifdef DIRECT3D_12

        hr = direct3d_device_11_priv->QueryInterface(IID_PPV_ARGS(&dxgi_device));
        if(FAILED(hr)){
            //Handle Error!
        };

        hr = direct2d_factory->CreateDevice(dxgi_device.get(),&direct2d_device);
        if(FAILED(hr)){
            //Handle Error!
        };

        hr = DCompositionCreateDevice(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device_1));
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = DCompositionCreateDevice3(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device_3));
        if(FAILED(hr)){
            //Handle Error!
        };
        
        #else
        
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
        hr = DCompositionCreateDevice(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device_1));
        if(FAILED(hr)){
            //Handle Error!
        };
        hr = DCompositionCreateDevice3(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device_3));
        if(FAILED(hr)){
            //Handle Error!
        };

        #endif
        

    };
    DXBDCompositionDevice::~DXBDCompositionDevice(){
        Core::SafeRelease(&dcomp_device_1);
        Core::SafeRelease(&dcomp_device_3);
        Core::SafeRelease(&direct2d_device);
        Core::SafeRelease(&dxgi_factory);
        Core::SafeRelease(&dxgi_adapter);
        Core::SafeRelease(&direct2d_factory);
        Core::SafeRelease(&direct3d_device);
        #ifdef DIRECT3D_12
        Core::SafeRelease(&direct3d_device_11_priv);
        #endif
    };

    Core::SharedPtr<BDCompositionDevice> DXBDCompositionDevice::Create(){
        return std::make_shared<DXBDCompositionDevice>();
    };
    // Core::SharedPtr<BDCompositionViewRenderTarget> DXBDCompositionDevice::makeViewRenderTarget(Layer *layer){
    //     // MessageBoxA(GetForegroundWindow(),"Making DX Render Target","",MB_OK);
    //     return DXBDCompositionViewRenderTarget::Create(this,(Native::Win::HWNDItem *));
    // };
    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionDevice::makeImageRenderTarget(Core::Rect &size){
        return DXBDCompositionImageRenderTarget::Create(this,size);
    };
    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionDevice::makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img){
        DXBDCompositionImage *dxImg = (DXBDCompositionImage *)img.get();
        ID2D1DeviceContext *device_context;
        HRESULT hr;
        hr = direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&device_context);
        if(FAILED(hr)){

        };
        return DXBDCompositionImageRenderTarget::Create(this,dxImg->rect);
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
    Core::SharedPtr<BDCompositionVisualTree> DXBDCompositionDevice::createVisualTree(){
        return DCVisualTree::Create(this);
    };
    void DXBDCompositionDevice::renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> &visualTree, ViewRenderTarget *view){
        HRESULT hr;
        DCVisualTree *tree = (DCVisualTree *)visualTree.get();
        Native::Win::HWNDItem * hwndItem = (Native::Win::HWNDItem *)view->getNativePtr();
        dcomp_device_3->Commit();
        IDCompositionTarget *target;
        if(!tree->hwndTarget.get()){
           hr = dcomp_device_1->CreateTargetForHwnd(hwndItem->getHandle(),FALSE,&target);
           if(FAILED(hr)){
               // Handle Error
               // Dont't attach to hwndtarget!
           };
           tree->hwndTarget = target;
        }
        else 
            target = tree->hwndTarget.get();

        DCVisualTree::Visual *rootV = (DCVisualTree::Visual *)tree->root_v.get();

        target->SetRoot(rootV->visual);

         dcomp_device_1->Commit();

    };
};