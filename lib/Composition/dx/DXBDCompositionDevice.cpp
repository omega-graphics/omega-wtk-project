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
        queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT;
        queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
        queue_desc.NodeMask = direct3d_device->GetNodeCount();

        hr = direct3d_device->CreateCommandQueue(&queue_desc,IID_PPV_ARGS(&direct3d_command_queue));
        if(FAILED(hr)){
            MessageBoxA(HWND_DESKTOP,"Failed to Create Command Queue!",NULL,MB_OK);
        };

        hr = D3D11On12CreateDevice(direct3d_device.get(),D3D11_CREATE_DEVICE_BGRA_SUPPORT,feature_level,ARRAYSIZE(feature_level),(IUnknown *const*)&direct3d_command_queue,1,direct3d_device->GetNodeCount(),&direct3d_device_11_priv,NULL,NULL);
        if(FAILED(hr)){
            MessageBoxA(HWND_DESKTOP,"Failed to create D3D11on12Device",NULL,MB_OK);
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
            MessageBoxA(HWND_DESKTOP,"Failed to get DXGIDevice",NULL,MB_OK);
        };

        hr = direct2d_factory->CreateDevice(dxgi_device.get(),&direct2d_device);
        if(FAILED(hr)){
            MessageBoxA(HWND_DESKTOP,"Failed to create Direct2d Device",NULL,MB_OK);
            //Handle Error!
        };

        hr = DCompositionCreateDevice3(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device_1));
        if(FAILED(hr)){
            //Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to create  DComp Desktop Device",NULL,MB_OK);
        };
        hr = dcomp_device_1->QueryInterface(&dcomp_device_2);
        if(FAILED(hr)){
            //Handle Error!
            std::stringstream ss;
            ss << std::hex << hr;
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to get  DComp Device 3 from DComp Desktop Device ERROR:") + ss.str()).c_str(),NULL,MB_OK);
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
        // Core::SafeRelease(&dcomp_device_2);
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
    Core::SharedPtr<BDCompositionDeviceContext> DXBDCompositionDevice::createContext(){
        return DXBDCompositionDeviceContext::Create(this);
    };

    DXBDCompositionDeviceContext::DXBDCompositionDeviceContext(DXBDCompositionDevice *device):device(device){};

    Core::SharedPtr<BDCompositionDeviceContext> DXBDCompositionDeviceContext::Create(DXBDCompositionDevice *device){
        return std::make_shared<DXBDCompositionDeviceContext>(device);
    };
    // Core::SharedPtr<BDCompositionViewRenderTarget> DXBDCompositionDevice::makeViewRenderTarget(Layer *layer){
    //     // MessageBoxA(GetForegroundWindow(),"Making DX Render Target","",MB_OK);
    //     return DXBDCompositionViewRenderTarget::Create(this,(Native::Win::HWNDItem *));
    // };
    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionDeviceContext::makeImageRenderTarget(Core::Rect &size){
        return DXBDCompositionImageRenderTarget::Create(device,size);
    };
    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionDeviceContext::makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> &img){
        DXBDCompositionImage *dxImg = (DXBDCompositionImage *)img.get();
        ID2D1DeviceContext *device_context;
        HRESULT hr;
        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&device_context);
        if(FAILED(hr)){

        };
        return DXBDCompositionImageRenderTarget::Create(device,dxImg->rect);
    };
    Core::SharedPtr<BDCompositionFontFactory> DXBDCompositionDevice::createFontFactory(){
        HRESULT hr;
        IDWriteFactory *factory;
        hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),(IUnknown **)&factory);
        if(FAILED(hr)){

        };
        return DXBDCompositionFontFactory::Create(factory);
    };
    void DXBDCompositionDeviceContext::destroyTarget(Layer *layer, Core::SharedPtr<BDCompositionRenderTarget> &target){

    };
    Core::SharedPtr<BDCompositionVisualTree> DXBDCompositionDeviceContext::createVisualTree(){
        return DCVisualTree::Create(device);
    };
    void DXBDCompositionDeviceContext::renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> &visualTree, ViewRenderTarget *view,bool updatePass){
        HRESULT hr;
        DCVisualTree *tree = (DCVisualTree *)visualTree.get();
        Native::Win::HWNDItem * hwndItem = (Native::Win::HWNDItem *)view->getNativePtr();
        IDCompositionTarget *target;
        if(!tree->hwndTarget.get()){
           hr = device->dcomp_device_1->CreateTargetForHwnd(hwndItem->hwnd,FALSE,&target);
           if(FAILED(hr)){
               MessageBoxA(HWND_DESKTOP,"Failed to Create Target For HWND","NOTE",MB_OK);
               // Handle Error
               // Dont't attach to hwndtarget!
           };
           tree->hwndTarget = target;
        }
        else 
            target = tree->hwndTarget.get();

        DCVisualTree::Visual *rootV = (DCVisualTree::Visual *)tree->root_v.get();
        UINT rootVHeight = ((DXBDCompositionImageRenderTarget *)rootV->img.get())->rect.dimen.minHeight;
        hr = rootV->visual->SetOpacityMode(DCOMPOSITION_OPACITY_MODE_LAYER);
        hr = target->SetRoot(rootV->visual);

        UINT dpi = GetDpiForWindow(((Native::Win::HWNDItem *)view->getNativePtr())->hwnd);
        FLOAT scaleFactor = FLOAT(dpi)/96.f;

        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << hr;
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to Set Root Visual:") + ss.str()).c_str(),NULL,MB_OK);
        }
        else {
            auto body_it = tree->body.begin();
            while(body_it != tree->body.end()){
                auto childV = (DCVisualTree::Visual *)body_it->get();
                DXBDCompositionImageRenderTarget *__img = (DXBDCompositionImageRenderTarget *)childV->img.get();

                if(__img->dropShadow){
                    IDCompositionVisual2 *shadowVisual;
                    LayerEffect::DropShadowParams *params = (LayerEffect::DropShadowParams *)__img->dropShadow->params;
                    device->dcomp_device_1->CreateVisual(&shadowVisual);
                    hr = shadowVisual->SetContent(__img->dxgi_swap_chain_1.get());
                    if(FAILED(hr)){
                        MessageBoxA(GetForegroundWindow(),"Failed to set DComp Visual Content for Shadow",NULL,MB_OK);
                    };
                    IDCompositionShadowEffect *shadowEffect;
                     hr = device->dcomp_device_2->CreateShadowEffect(&shadowEffect);
                     if(FAILED(hr)){
                        MessageBoxA(GetForegroundWindow(),"Failed to create Shadow Effect",NULL,MB_OK);
                    };
                    shadowEffect->SetStandardDeviation((params->radius/3.f));
                    auto d2d1_color = color_to_d2d1_color(params->color);

                    shadowEffect->SetColor(D2D1::Vector4F(d2d1_color.r,d2d1_color.g,d2d1_color.b,d2d1_color.a));

                    shadowEffect->SetAlpha(params->opacity * 0.7f);

                    hr = shadowVisual->SetEffect(shadowEffect);
                    if(FAILED(hr)){

                    };

                    hr = shadowVisual->SetTransform(D2D1::Matrix3x2F::Translation(params->x_offset * scaleFactor,(-params->y_offset) * scaleFactor));

                    hr = shadowVisual->SetOffsetX((childV->pos.x) * scaleFactor);
                    hr = shadowVisual->SetOffsetY((rootVHeight - childV->pos.y - __img->rect.dimen.minHeight) * scaleFactor);

                    hr = shadowVisual->SetOpacityMode(DCOMPOSITION_OPACITY_MODE_INHERIT);

                    hr = childV->visual->SetOpacityMode(DCOMPOSITION_OPACITY_MODE_INHERIT);
                    hr = childV->visual->SetOffsetX((childV->pos.x) * scaleFactor);
                    hr = childV->visual->SetOffsetY((rootVHeight - childV->pos.y - __img->rect.dimen.minHeight) * scaleFactor);

                    // hr = shadowVisual->AddVisual(childV->visual,FALSE,NULL);
                   

                    hr = rootV->visual->AddVisual(shadowVisual,FALSE,NULL);
                    if(FAILED(hr)){

                    };

                    hr = rootV->visual->AddVisual(childV->visual,TRUE,shadowVisual);
                    if(FAILED(hr)){

                    };

                    // Core::SharedPtr<BDCompositionImageRenderTarget> img = makeImageRenderTarget(__img->rect);
                    // Composition::Color b(Composition::Color::Black,0x00);
                    // img->clear(b);
                    // DXBDCompositionImageRenderTarget *__shadow_img = (DXBDCompositionImageRenderTarget *)img.get();
                    // ID2D1Bitmap1 *bitmap =  __shadow_img->first_target.get();
                    // ID2D1Effect *shadow;
                    // hr = __shadow_img->direct2d_device_context->CreateEffect(CLSID_D2D1Shadow,&shadow);
                    // if(FAILED(hr)){

                    // };
                    // shadow->SetInput(0,__img->direct2d_bitmap.get());
                    // shadow->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION,(params->radius/3.f));
                    // auto d2d1_color = color_to_d2d1_color(params->color);
                    // shadow->SetValue(D2D1_SHADOW_PROP_COLOR,D2D1::Vector4F(d2d1_color.r,d2d1_color.g,d2d1_color.r,d2d1_color.a));
                    // __shadow_img->direct2d_device_context->DrawImage(shadow);
                    // img->commitEffects();
                    // img->commit();
                    // hr = shadowVisual->SetContent(__shadow_img->dxgi_swap_chain_1.get());
                    // if(FAILED(hr)){

                    // };

                    // hr = shadowVisual->AddVisual(childV->visual,FALSE,NULL);
                    // if(FAILED(hr)){

                    // };
                }
                else {
                
                    hr = childV->visual->SetOffsetX(childV->pos.x * scaleFactor);
                    hr = childV->visual->SetOffsetY((rootVHeight - childV->pos.y - __img->rect.dimen.minHeight) * scaleFactor);


                

                    hr = childV->visual->SetOpacityMode(DCOMPOSITION_OPACITY_MODE_INHERIT);

                    if(FAILED(hr)){

                    };

                    hr = rootV->visual->AddVisual(childV->visual,FALSE,NULL);

                };
                
                ++body_it;
            };
        };

        device->dcomp_device_1->Commit();
    };
};