#include "DXBDCompositionRenderTarget.h"

namespace OmegaWTK::Composition {

    HRESULT hr;

    D2D1::ColorF color_to_d2d1_color(Color & color){
        #define EIGHTBIT_TO_FLOAT(num) (num/255.f)
        return D2D1::ColorF(EIGHTBIT_TO_FLOAT(color.r),EIGHTBIT_TO_FLOAT(color.g),EIGHTBIT_TO_FLOAT(color.b),EIGHTBIT_TO_FLOAT(color.a));
    };

    RECT core_rect_to_win_rect(Core::Rect &rect,HWND parent){
        #define DEFAULT_DPI 96.f
        UINT dpi = GetDpiForWindow(parent);

        // MessageBox(GetForegroundWindow(),(std::string("DPI :") + std::to_string(dpi)).c_str(),"Note",MB_OK);
        RECT rectres;
        
        FLOAT sacleFactor = FLOAT(dpi)/DEFAULT_DPI;

        RECT parent_wnd_rect;
        GetClientRect(parent,&parent_wnd_rect);
        // FLOAT sacleFactor = 1.0;

        rectres.left = rect.pos.x;
        rectres.right = (rectres.left + (rect.dimen.minWidth) * sacleFactor);

         rectres.bottom = ((parent_wnd_rect.bottom) - rect.pos.y * sacleFactor);
         rectres.top = rectres.bottom - (rect.dimen.minHeight * sacleFactor);
        
        return rectres;
    };
    // /// No DPI SCALE!
    RECT core_rect_to_win_rect_dip(Core::Rect &rect,HWND parent){
        #define DEFAULT_DPI 96.f
        UINT dpi = GetDpiForWindow(parent);

        // MessageBox(GetForegroundWindow(),(std::string("DPI :") + std::to_string(dpi)).c_str(),"Note",MB_OK);
        RECT rectres;
        
        FLOAT sacleFactor = FLOAT(dpi)/DEFAULT_DPI;

        RECT parent_wnd_rect;
        GetClientRect(parent,&parent_wnd_rect);
        // FLOAT sacleFactor = 1.0;

        rectres.left = rect.pos.x;
        rectres.right = (rectres.left + (rect.dimen.minWidth));

         rectres.bottom = ((parent_wnd_rect.bottom / sacleFactor) - rect.pos.y);
         rectres.top = rectres.bottom - (rect.dimen.minHeight);
        
        return rectres;
    };

    ID2D1Brush *omegawtk_brush_to_d2d1_brush(Brush & brush,ID2D1DeviceContext *device_context){
        ID2D1Brush *res;
        if(brush.isColor){
            ID2D1SolidColorBrush *br;
            hr = device_context->CreateSolidColorBrush(color_to_d2d1_color(brush.color),&br);
            // if(FAILED(hr)){
            //     MessageBoxA(GetForegroundWindow(),"Failed to Create Color Brush","NOTE",MB_OK);
            //     /// Color Failed!
            // };
            res = br;
        }
        else if(brush.isGradient){
            res = nullptr;
        }
        else {
            // MessageBoxA(GetForegroundWindow(),"Unknown Brush Type!","NOTE",MB_OK);
            res = nullptr;
        };
        return res;
    };

    DXBDCompositionRenderTarget::DXBDCompositionRenderTarget(DXBDCompositionDevice *device,Native::Win::HWNDItem *hwndItem):device(device),hwndItem(hwndItem){
        redoSwapChain();
    };
    
    Core::SharedPtr<BDCompositionRenderTarget> DXBDCompositionRenderTarget::Create(DXBDCompositionDevice *device, Native::Win::HWNDItem *item){
        return std::make_shared<DXBDCompositionRenderTarget>(DXBDCompositionRenderTarget(device,item));
    };
    
    DXBDCompositionRenderTarget::~DXBDCompositionRenderTarget(){
        Core::SafeRelease(&dcomp_target);
        Core::SafeRelease(&direct2d_bitmap);
        Core::SafeRelease(&direct2d_device_context);
        Core::SafeRelease(&dxgi_surface);
        Core::SafeRelease(&dxgi_swap_chain);
    };

    void DXBDCompositionRenderTarget::redoSwapChain(){
        DXGI_SWAP_CHAIN_DESC1 desc {0};
        desc.BufferCount = 2;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Height = 0;
        desc.Width = 0;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Scaling = DXGI_SCALING_NONE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        desc.Stereo = FALSE;
        desc.Flags = 0;

        hr = device->dxgi_factory->CreateSwapChainForHwnd(device->direct3d_device.get(),hwndItem->getHandle(),&desc,NULL,NULL,&dxgi_swap_chain);
        if(FAILED(hr)){
            /// Handle Error!
        };
        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
        };

        hr = dxgi_swap_chain->GetBuffer(0,IID_PPV_ARGS(&dxgi_surface));
        if(FAILED(hr)){
            /// Handle Error!
        };
        UINT dpi = GetDpiForWindow(hwndItem->getHandle());
        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
        };

        direct2d_device_context->SetTarget(direct2d_bitmap.get());

        recreateSwapChain = false;
        recreateDeviceContext = false;
    };

    void DXBDCompositionRenderTarget::redoDeviceContext(){
        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
        };
        UINT dpi = GetDpiForWindow(hwndItem->getHandle());
        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
        };

        direct2d_device_context->SetTarget(direct2d_bitmap.get());


        recreateSwapChain = false;
        recreateDeviceContext = false;
    };

    void DXBDCompositionRenderTarget::clear(Color &clearColor){
        direct2d_device_context->BeginDraw();
        direct2d_device_context->SetTransform(D2D1::IdentityMatrix());
        direct2d_device_context->Clear(color_to_d2d1_color(clearColor));
    };
    void DXBDCompositionRenderTarget::frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned width){
        // if(hasBrush(brush)){
            ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
            RECT rc = core_rect_to_win_rect(rect,hwndItem->getHandle());
            UINT dpi = GetDpiForWindow(hwndItem->getHandle());
            FLOAT scaleFactor = FLOAT(dpi)/96.f;
            direct2d_device_context->DrawRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),_brush,FLOAT(width) * scaleFactor);
        // };
    };
    void DXBDCompositionRenderTarget::fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush){
        // if(hasBrush(brush)){
            ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
            RECT rc = core_rect_to_win_rect(rect,hwndItem->getHandle());
            direct2d_device_context->FillRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),_brush);
            Core::SafeRelease(&_brush);
        // };
    };
    void DXBDCompositionRenderTarget::commit(){
        DXGI_PRESENT_PARAMETERS params;
        params.DirtyRectsCount = NULL;
        params.pDirtyRects = NULL;
        params.pScrollOffset = NULL;
        params.pScrollRect = NULL;
        hr = direct2d_device_context->EndDraw();
        if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET){
            Core::SafeRelease(&direct2d_device_context);
            Core::SafeRelease(&direct2d_bitmap);
            recreateDeviceContext = true;
            recreateSwapChain = false;
        }
        else {
            hr = dxgi_swap_chain->Present1(1,0,&params);
            if(FAILED(hr) || hr == DXGI_STATUS_OCCLUDED ){
                Core::SafeRelease(&dxgi_swap_chain);
                Core::SafeRelease(&dxgi_surface);
                Core::SafeRelease(&direct2d_device_context);
                Core::SafeRelease(&direct2d_bitmap);
                recreateSwapChain = recreateDeviceContext = true;
            }
            else {
                recreateSwapChain = false;
                recreateDeviceContext = false;
            }
        };
    };

};