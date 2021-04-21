#include "DXBDCompositionRenderTarget.h"
#include "DXBDCompositionImage.h"
#include "DXBDCompositionFontFactory.h"

namespace OmegaWTK::Composition {

    HRESULT hr;

    D2D1::ColorF color_to_d2d1_color(Color & color){
        #define EIGHTBIT_TO_FLOAT(num) (num/255.f)
        return D2D1::ColorF(EIGHTBIT_TO_FLOAT(color.r),EIGHTBIT_TO_FLOAT(color.g),EIGHTBIT_TO_FLOAT(color.b),EIGHTBIT_TO_FLOAT(color.a));
    };
    /// Convert from Core::Rect to RECT using the HWND parent
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
    
    /// Convert from Core::Rect to RECT using dpi and parent Core::Rect 
    /// NOTE: (Assumes parent_rect is NOT DPI SCALED!!)
    RECT core_rect_to_win_rect_from_parent_core_rect(Core::Rect &rect,UINT & dpi,Core::Rect &parent_rect){
        #define DEFAULT_DPI 96.f
        // UINT dpi = GetDpiForWindow(parent);

        // MessageBox(GetForegroundWindow(),(std::string("DPI :") + std::to_string(dpi)).c_str(),"Note",MB_OK);
        RECT rectres;
        
        FLOAT scaleFactor = FLOAT(dpi)/DEFAULT_DPI;

        // FLOAT sacleFactor = 1.0;

        rectres.left = rect.pos.x * scaleFactor;
        rectres.right = (rectres.left + (rect.dimen.minWidth) * scaleFactor);

        rectres.bottom = ((parent_rect.dimen.minHeight * scaleFactor) - rect.pos.y * scaleFactor);
        rectres.top = rectres.bottom - (rect.dimen.minHeight * scaleFactor);
        
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

    DXBDCompositionViewRenderTarget::DXBDCompositionViewRenderTarget(DXBDCompositionDevice *device,Native::Win::HWNDItem *hwndItem):IDXBDCompositionRenderTarget<BDCompositionViewRenderTarget>(device),hwndItem(hwndItem){
        redoSwapChain();
    };

    Core::SharedPtr<BDCompositionViewRenderTarget> DXBDCompositionViewRenderTarget::Create(DXBDCompositionDevice *device, Native::Win::HWNDItem *item){
        return std::make_shared<DXBDCompositionViewRenderTarget>(DXBDCompositionViewRenderTarget(device,item));
    };

    void DXBDCompositionViewRenderTarget::redoSwapChain(){
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
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        IDXGISwapChain1 *swapC;
        #ifdef DIRECT3D_12
        hr = device->dxgi_factory->CreateSwapChainForHwnd(device->direct3d_command_queue.get(),hwndItem->getHandle(),&desc,NULL,NULL,&swapC);
        if(FAILED(hr)){
            /// Handle Error!
        };
        hr = swapC->QueryInterface(&dxgi_swap_chain);
        if(FAILED(hr)){
            /// Handle Error!
        };

        hr = dxgi_swap_chain->SetMaximumFrameLatency(3);
        if(FAILED(hr)){
            /// Handle Error!
        };
        #else 
        hr = device->dxgi_factory->CreateSwapChainForHwnd(device->direct3d_device.get(),hwndItem->getHandle(),&desc,NULL,NULL,&dxgi_swap_chain);
        if(FAILED(hr)){
            /// Handle Error!
        };
        #endif
        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
        };

        hr = dxgi_swap_chain->GetBuffer(0,IID_PPV_ARGS(&dxgi_surface));
        if(FAILED(hr)){
            /// Handle Error!
        };
        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
        };

        direct2d_device_context->SetTarget(direct2d_bitmap.get());

        recreateSwapChain = false;
        recreateDeviceContext = false;
        newTarget = true;
    };

    void DXBDCompositionViewRenderTarget::redoDeviceContext(){
        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
        };
        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
        };

        direct2d_device_context->SetTarget(direct2d_bitmap.get());

        
        recreateSwapChain = false;
        recreateDeviceContext = false;
        newTarget = true;
    };

    void DXBDCompositionViewRenderTarget::frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned width){
        // if(hasBrush(brush)){
            ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
            RECT rc = core_rect_to_win_rect(rect,hwndItem->getHandle());
            FLOAT scaleFactor = FLOAT(dpi)/96.f;
            direct2d_device_context->DrawRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),_brush,FLOAT(width) * scaleFactor);
        // };
    };
    void DXBDCompositionViewRenderTarget::fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush){
        // if(hasBrush(brush)){
            ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
            RECT rc = core_rect_to_win_rect(rect,hwndItem->getHandle());
            direct2d_device_context->FillRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),_brush);
            Core::SafeRelease(&_brush);
        // };
    };
    void DXBDCompositionViewRenderTarget::frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned width){
        ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        RECT rc = core_rect_to_win_rect(rect,hwndItem->getHandle());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        direct2d_device_context->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),FLOAT(rect.radius_x) * scaleFactor,FLOAT(rect.radius_y) * scaleFactor),_brush,FLOAT(width) * scaleFactor);
        Core::SafeRelease(&_brush);
    };
    void DXBDCompositionViewRenderTarget::fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush){
        ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        RECT rc = core_rect_to_win_rect(rect,hwndItem->getHandle());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        direct2d_device_context->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),FLOAT(rect.radius_x) * scaleFactor,FLOAT(rect.radius_y) * scaleFactor),_brush);
        Core::SafeRelease(&_brush);
    };

    Core::SharedPtr<BDCompositionImage> DXBDCompositionViewRenderTarget::createImageFromBitmapImage(Core::SharedPtr<Media::BitmapImage> &img,Core::Rect &newSize,unsigned v_id){
        auto & header = img->header;
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        ID2D1Bitmap *bitmap;
        ID2D1Image *res, *res2;
        hr = direct2d_device_context->CreateBitmap(D2D1::SizeU(header->width,header->height),img->data,header->stride,D2D1::BitmapProperties(D2D1::PixelFormat(computePixelFormat(header->bitDepth,header->channels,img->sRGB),D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&bitmap);
        if(FAILED(hr)){

        };
        ID2D1Effect *alphaFix, *scaleFix;
        hr = direct2d_device_context->CreateEffect(CLSID_D2D1Premultiply,&alphaFix);
        if(FAILED(hr)){

        };
        hr = direct2d_device_context->CreateEffect(CLSID_D2D1Scale,&scaleFix);
        if(FAILED(hr)){

        };
        scaleFix->SetValue(D2D1_SCALE_PROP_INTERPOLATION_MODE,D2D1_SCALE_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR);
        scaleFix->SetValue(D2D1_SCALE_PROP_SCALE,D2D1::Vector2F(FLOAT(newSize.dimen.minWidth * scaleFactor)/FLOAT(header->width),FLOAT(newSize.dimen.minHeight * scaleFactor)/FLOAT(header->height)));

        scaleFix->SetInput(0,bitmap);
        scaleFix->GetOutput(&res);
        alphaFix->SetInput(0,res);
        alphaFix->GetOutput(&res2);
        
        auto image = DXBDCompositionImage::Create(img,newSize,this,res2);
        images.insert(std::make_pair(v_id,image));
        return image;
    };
    void DXBDCompositionViewRenderTarget::drawImage(Core::SharedPtr<BDCompositionImage> &img,Core::Position pos){
        DXBDCompositionImage *compImg = reinterpret_cast<DXBDCompositionImage *>(img.get());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        ID2D1Image * img_ref = compImg->native_image.get();
        RECT rc;
        GetClientRect(hwndItem->getHandle(),&rc);
        auto & rect = compImg->rect;
        direct2d_device_context->DrawImage(img_ref,D2D1::Point2F(pos.x * scaleFactor,rc.bottom - (rect.dimen.minHeight * scaleFactor) - (pos.y * scaleFactor)));
    };
    void DXBDCompositionViewRenderTarget::drawText(Core::SharedPtr<TextRect> &textRect, Core::SharedPtr<Brush> &brush){
        ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        IDWriteTextLayout *textLayout = (IDWriteTextLayout *)textRect->getNative();
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        RECT rc;
        GetClientRect(hwndItem->getHandle(),&rc);
        auto & pos = textRect->rect.pos;
        auto & rect = textRect->rect;

        DWRITE_TEXT_RANGE r;
        r.startPosition = 0;
        r.length = textRect->getString().size();
        textLayout->SetDrawingEffect(_brush,r);
        
       direct2d_device_context->DrawTextLayout(D2D1::Point2F(pos.x * scaleFactor,rc.bottom - (rect.dimen.minHeight * scaleFactor) - (pos.y * scaleFactor)),textLayout,_brush);
    };
    

    void DXBDCompositionViewRenderTarget::commit(){
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
                newTarget = false;
            }
        };
    };

    void DXBDCompositionViewRenderTarget::resizeBuffers(Core::Rect &newRect){

    };

    DXBDCompositionViewRenderTarget::~DXBDCompositionViewRenderTarget(){
        Core::SafeRelease(&dxgi_surface);
        Core::SafeRelease(&dxgi_swap_chain);
        Core::SafeRelease(&direct2d_bitmap);
    };

    DXBDCompositionImageRenderTarget::DXBDCompositionImageRenderTarget(DXBDCompositionDevice *device,Core::Rect &rect):IDXBDCompositionRenderTarget<BDCompositionImageRenderTarget>(device),rect(rect){
        redoSwapChain();
    };

    void DXBDCompositionImageRenderTarget::redoDeviceContext(){
       FLOAT scaleFactor = FLOAT(dpi)/96.f;

        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
        };
        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to Create Main Bitmap",NULL, MB_OK);
        };

        // // delete imgData;
        // // Core::SafeRelease(&first_target);

        // imgData = new unsigned char[rect.dimen.minWidth * 4 * rect.dimen.minHeight * (scaleFactor * scaleFactor)];
        // hr = direct2d_device_context->CreateBitmap(D2D1::SizeU(rect.dimen.minWidth * scaleFactor,rect.dimen.minHeight * scaleFactor),imgData,rect.dimen.minWidth * 4 * scaleFactor,D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&first_target);
        // if(FAILED(hr)){
        //     /// Handle Error!
        //     MessageBoxA(HWND_DESKTOP,"Failed to Create Bitmap",NULL, MB_OK);
        // };

        direct2d_device_context->SetTarget(direct2d_bitmap.get());

        
        recreateSwapChain = false;
        recreateDeviceContext = false;
        newTarget = true;
        
    };

    void DXBDCompositionImageRenderTarget::redoSwapChain(){
        dpi = GetDpiForWindow(GetForegroundWindow());
       FLOAT scaleFactor = FLOAT(dpi)/96.f;

        DXGI_SWAP_CHAIN_DESC1 desc {0};
        desc.BufferCount = 2;
        desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Height = rect.dimen.minHeight * scaleFactor;
        desc.Width = rect.dimen.minWidth * scaleFactor;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Scaling = DXGI_SCALING_STRETCH;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        desc.Stereo = FALSE;
        desc.Flags = 0;
        // MessageBoxA(HWND_DESKTOP,"Creating Swap Chain",NULL, MB_OK);
        #ifdef DIRECT3D_12
        hr = device->dxgi_factory->CreateSwapChainForComposition(device->direct3d_device_11_priv.get(),&desc,NULL,&dxgi_swap_chain_1);
        
        if(FAILED(hr)){
            MessageBoxA(HWND_DESKTOP,"Failed to Create SwapChain!",NULL, MB_OK);
            /// Handle Error!
        }
        else {
            // MessageBoxA(HWND_DESKTOP,"Created Swap Chain",NULL, MB_OK);
        };

        hr = dxgi_swap_chain_1->QueryInterface(&dxgi_swap_chain_3);
        if(FAILED(hr)){
            MessageBoxA(HWND_DESKTOP,"Failed to get DXGISwapChain2!",NULL, MB_OK);
            /// Handle Error!
        }
        else {
            //  MessageBoxA(HWND_DESKTOP,"Got DXGISwapChain3",NULL, MB_OK);
        };
        // hr = dxgi_swap_chain_2->SetMaximumFrameLatency(3);
        // if(FAILED(hr)){
        //     MessageBoxA(HWND_DESKTOP,"Could not Set Frame Latency",NULL, MB_OK);
        // };
        #else 
        hr = device->dxgi_factory->CreateSwapChainForComposition(device->direct3d_device.get(),&desc,NULL,&dxgi_swap_chain);

        if(FAILED(hr)){
            /// Handle Error!
        };
        #endif
        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to Create Device Context",NULL, MB_OK);
        };

        // hr = device->dcomp_device_1->CreateSurface(rect.dimen.minWidth * scaleFactor,rect.dimen.minHeight * scaleFactor, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ALPHA_MODE_PREMULTIPLIED,&dcomp_surface);
        //  if(FAILED(hr)){
        //     /// Handle Error!
        //     MessageBoxA(HWND_DESKTOP,"Failed to create Compostion Surface",NULL, MB_OK);
        // };
        // POINT pointOffset = { };
        // hr = dcomp_surface->BeginDraw(NULL,IID_PPV_ARGS(&dxgi_surface),&pointOffset);
        UINT bufferIdx = dxgi_swap_chain_3->GetCurrentBackBufferIndex();
        
        hr = dxgi_swap_chain_3->GetBuffer(bufferIdx,IID_PPV_ARGS(&dxgi_surface));
        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << hr;
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to Begin Draw Surface.. ERROR:") + ss.str()).c_str(),NULL, MB_OK);
        };

        // hr = direct3d_res->QueryInterface(&dxgi_surface);
        // if(FAILED(hr)){
        //     std::stringstream ss;
        //     ss << std::hex << hr;
        //     MessageBoxA(HWND_DESKTOP,(std::string("Failed to get DXGI Surface from ID3D11Texture2D ERROR:") + ss.str()).c_str(),NULL, MB_OK);
        // };

        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to Create Bitmap from DXGI Surface",NULL, MB_OK);
        };
        imgData = new unsigned char[rect.dimen.minWidth * 4 * rect.dimen.minHeight * (scaleFactor * scaleFactor)];
        hr = direct2d_device_context->CreateBitmap(D2D1::SizeU(rect.dimen.minWidth * scaleFactor,rect.dimen.minHeight * scaleFactor),imgData,rect.dimen.minWidth * 4 * scaleFactor,D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&first_target);
        if(FAILED(hr)){
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to Create Bitmap",NULL, MB_OK);
        };

        direct2d_device_context->SetTarget(first_target.get());

        recreateSwapChain = false;
        recreateDeviceContext = false;
        newTarget = true;
    };

    void DXBDCompositionImageRenderTarget::redoSwapChainWithNewSize(Core::Rect &newRect){
        rect = newRect;
        redoSwapChain();
    };

    void DXBDCompositionImageRenderTarget::resizeBuffers(Core::Rect &newRect){
        bool makeNewBitmap = false;
        if((rect.dimen.minHeight < newRect.dimen.minHeight) || (rect.dimen.minWidth < newRect.dimen.minWidth)){
            makeNewBitmap = true;
        };
        rect = newRect;
        dpi = GetDpiForWindow(GetForegroundWindow());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        HRESULT hr;

        Core::SafeRelease(&dxgi_surface);
        Core::SafeRelease(&direct2d_bitmap);
        Core::SafeRelease(&direct2d_device_context);

        hr = dxgi_swap_chain_3->ResizeBuffers(0,int(rect.dimen.minWidth * scaleFactor),int(rect.dimen.minHeight * scaleFactor), DXGI_FORMAT_UNKNOWN,0);
        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << "ERROR:" << hr;

            MessageBoxA(HWND_DESKTOP,ss.str().c_str(),NULL,MB_OK);
            exit(1);
        };

        hr = device->direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&direct2d_device_context);
        if(FAILED(hr)){
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to Create Device Context",NULL, MB_OK);
        };

        if(makeNewBitmap){

            delete imgData;
            Core::SafeRelease(&first_target);

            imgData = new Byte[rect.dimen.minWidth * 4 * rect.dimen.minHeight * (scaleFactor * scaleFactor)];
            hr = direct2d_device_context->CreateBitmap(D2D1::SizeU(rect.dimen.minWidth * scaleFactor,rect.dimen.minHeight * scaleFactor),imgData,rect.dimen.minWidth * 4 * scaleFactor,D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&first_target);
            if(FAILED(hr)){
                /// Handle Error!
                MessageBoxA(HWND_DESKTOP,"Failed to Create Bitmap",NULL, MB_OK);
            };

        }


        /// Set Target to Bitmap (to be drawn to Back Buffer)
        direct2d_device_context->SetTarget(first_target.get());


        UINT idx = dxgi_swap_chain_3->GetCurrentBackBufferIndex();
        hr = dxgi_swap_chain_3->GetBuffer(idx,IID_PPV_ARGS(&dxgi_surface));

        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << hr;
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to Begin Draw Surface.. ERROR:") + ss.str()).c_str(),NULL, MB_OK);
        };

        hr = direct2d_device_context->CreateBitmapFromDxgiSurface(dxgi_surface.get(),D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&direct2d_bitmap);
        if(FAILED(hr)){
            /// Handle Error!
            MessageBoxA(HWND_DESKTOP,"Failed to Create Bitmap from DXGI Surface",NULL, MB_OK);
        };

        recreateSwapChain = false;
        recreateDeviceContext = false;
        newTarget = true;
    };

    Core::SharedPtr<BDCompositionImageRenderTarget> DXBDCompositionImageRenderTarget::Create(DXBDCompositionDevice *device, Core::Rect &rect){
        return std::make_shared<DXBDCompositionImageRenderTarget>(device,rect);
    };

    void DXBDCompositionImageRenderTarget::frameRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush, unsigned int width){
         ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
         RECT rc = core_rect_to_win_rect_from_parent_core_rect(rect,dpi,this->rect);
         FLOAT scaleFactor = FLOAT(dpi)/96.f;
         direct2d_device_context->DrawRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),_brush,FLOAT(width) * scaleFactor);
         Core::SafeRelease(&_brush);
    };
    void DXBDCompositionImageRenderTarget::fillRect(Core::Rect &rect, Core::SharedPtr<Brush> &brush){
        ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        RECT rc = core_rect_to_win_rect_from_parent_core_rect(rect,dpi,this->rect);
        direct2d_device_context->FillRectangle(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),_brush);
        Core::SafeRelease(&_brush);
    };
    void DXBDCompositionImageRenderTarget::frameRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush, unsigned int width){
        ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        RECT rc = core_rect_to_win_rect_from_parent_core_rect(rect,dpi,this->rect);
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        direct2d_device_context->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),FLOAT(rect.radius_x) * scaleFactor,FLOAT(rect.radius_y) * scaleFactor),_brush,FLOAT(width) * scaleFactor);
        Core::SafeRelease(&_brush);
    };
    void DXBDCompositionImageRenderTarget::fillRoundedRect(Core::RoundedRect &rect, Core::SharedPtr<Brush> &brush){
         ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        RECT rc = core_rect_to_win_rect_from_parent_core_rect(rect,dpi,this->rect);
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        direct2d_device_context->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),FLOAT(rect.radius_x) * scaleFactor,FLOAT(rect.radius_y) * scaleFactor),_brush);
        Core::SafeRelease(&_brush);
    };
    Core::SharedPtr<BDCompositionImage> DXBDCompositionImageRenderTarget::createImageFromBitmapImage(Core::SharedPtr<Media::BitmapImage> &img, Core::Rect &newSize, unsigned int v_id){
         auto & header = img->header;
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        ID2D1Bitmap *bitmap;
        ID2D1Image *res, *res2;
        hr = direct2d_device_context->CreateBitmap(D2D1::SizeU(header->width,header->height),img->data,header->stride,D2D1::BitmapProperties(D2D1::PixelFormat(computePixelFormat(header->bitDepth,header->channels,img->sRGB),D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&bitmap);
        if(FAILED(hr)){
            
        };
        ID2D1Effect *alphaFix, *scaleFix;
        hr = direct2d_device_context->CreateEffect(CLSID_D2D1Premultiply,&alphaFix);
        if(FAILED(hr)){

        };
        hr = direct2d_device_context->CreateEffect(CLSID_D2D1Scale,&scaleFix);
        if(FAILED(hr)){

        };
        scaleFix->SetValue(D2D1_SCALE_PROP_INTERPOLATION_MODE,D2D1_SCALE_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR);
        scaleFix->SetValue(D2D1_SCALE_PROP_SCALE,D2D1::Vector2F(FLOAT(newSize.dimen.minWidth * scaleFactor)/FLOAT(header->width),FLOAT(newSize.dimen.minHeight * scaleFactor)/FLOAT(header->height)));

        scaleFix->SetInput(0,bitmap);
        scaleFix->GetOutput(&res);
        alphaFix->SetInput(0,res);
        alphaFix->GetOutput(&res2);
        
        auto image = DXBDCompositionImage::Create(img,newSize,this,res2);
        images.insert(std::make_pair(v_id,image));
        return image;
    };
    void DXBDCompositionImageRenderTarget::drawImage(Core::SharedPtr<BDCompositionImage> &img, Core::Position pos){
        DXBDCompositionImage *compImg = reinterpret_cast<DXBDCompositionImage *>(img.get());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        ID2D1Image * img_ref = compImg->native_image.get();
        Core::Rect &parent_rc = this->rect;
        
        auto & rect = compImg->rect;
        direct2d_device_context->DrawImage(img_ref,D2D1::Point2F(pos.x * scaleFactor,(parent_rc.dimen.minHeight * scaleFactor) - (rect.dimen.minHeight * scaleFactor) - (pos.y * scaleFactor)));
    };
    void DXBDCompositionImageRenderTarget::drawText(Core::SharedPtr<TextRect> &textRect, Core::SharedPtr<Brush> &brush){
        ID2D1Brush *_brush = omegawtk_brush_to_d2d1_brush(*brush,direct2d_device_context.get());
        IDWriteTextLayout *textLayout = (IDWriteTextLayout *)textRect->getNative();
        DWRITE_TEXT_RANGE r;
        r.startPosition = 0;
        r.length = textRect->getString().size();
        textLayout->SetDrawingEffect(_brush,r);
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
         Core::Rect &parent_rc = this->rect;
        auto & pos = textRect->rect.pos;
        auto & rect = textRect->rect;
        direct2d_device_context->DrawTextLayout(D2D1::Point2F(pos.x * scaleFactor,(parent_rc.dimen.minHeight * scaleFactor) - (rect.dimen.minHeight * scaleFactor) - (pos.y * scaleFactor)),textLayout,_brush);
    };

    void DXBDCompositionImageRenderTarget::applyEffect(LayerEffect *effect){
        switch (effect->type) {
            case LayerEffect::DropShadow : 
            {
                dropShadow = effect;
                break;
            }
            default: {
                effectBuffer.push(effect);
                break;
            }
        }
    };

    inline IID getIIDFromOmegaEffectType(LayerEffect::Type & type){
        IID res;
        switch (type) {
        case LayerEffect::DropShadow : {
            res = CLSID_D2D1Shadow;
            break;
        }
        case LayerEffect::GaussianBlur : {
            res = CLSID_D2D1GaussianBlur;
            break;
        }
        case LayerEffect::DirectionalBlur : {
            res = CLSID_D2D1DirectionalBlur;
            break;
        }
        default: {
            res = CLSID_NULL;
            break;
        }
        }
        return res;
    };

    void __applyEffect_interal(ID2D1Effect * _dx_effect,LayerEffect *effect,ID2D1Image *img,ID2D1Image  **out,IDCompositionDevice3 *dev){
        switch (effect->type) {
        case LayerEffect::GaussianBlur : {
            LayerEffect::GaussianBlurParams *params = (LayerEffect::GaussianBlurParams *)effect->params;
            _dx_effect->SetInput(0,img);
            _dx_effect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION,(params->radius/3.f));
            _dx_effect->GetOutput(out);
            break;
        }
        case LayerEffect::DirectionalBlur : {
            LayerEffect::DirectionalBlurParams *params = (LayerEffect::DirectionalBlurParams *)effect->params;
            _dx_effect->SetInput(0,img);
            _dx_effect->SetValue(D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION,(params->radius/3.f));
            _dx_effect->SetValue(D2D1_DIRECTIONALBLUR_PROP_ANGLE,params->angle);
            _dx_effect->GetOutput(out);
            break;
        }
        default:
            break;
        }
        Core::SafeRelease(&_dx_effect);
    };

    void DXBDCompositionImageRenderTarget::commitEffects(){
        HRESULT hr;
        hr = direct2d_device_context->EndDraw();
        if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET){
            /// Dont Apply Effects if Failed!
            MessageBoxA(GetForegroundWindow(),"Recreate Target",NULL,MB_OK);
            return;
        };

        ID2D1Image *temp1,*temp2;
        if(!effectBuffer.empty()) {
            auto first_effect = effectBuffer.front();
            effectBuffer.pop();
            ID2D1Effect *_dx_effect;
            IID iid = getIIDFromOmegaEffectType(first_effect->type);
            hr = direct2d_device_context->CreateEffect(iid,&_dx_effect);
            if(FAILED(hr)){

            };
            temp1 = first_target.get();
            __applyEffect_interal(_dx_effect,first_effect,temp1,&temp2,device->dcomp_device_2.get());
            temp1 = temp2;
            //  Core::SafeRelease(&temp1);
            //  Core::SafeRelease(&_dx_effect);
            // temp1 = temp2;
            // while(!effectBuffer.empty()){
            //     auto effect = effectBuffer.front();
            //     effectBuffer.pop();
            //     ID2D1Effect *_dx_effect;
            //     IID iid = getIIDFromOmegaEffectType(effect->type);
            //     hr = direct2d_device_context->CreateEffect(iid,&_dx_effect);
            //     if(FAILED(hr)){

            //     };
            //     __applyEffect_interal(_dx_effect,effect,temp1,temp2);
            //     // Core::SafeRelease(&temp1);
            //     // Core::SafeRelease(&_dx_effect);
            //     temp1 = temp2;
            // };
        }
        else {
            temp1 = first_target.get();
        }
        ID2D1Effect *premultiAlpha;
        hr = direct2d_device_context->CreateEffect(CLSID_D2D1Premultiply,&premultiAlpha);
        if(FAILED(hr)){

        };
        premultiAlpha->SetInput(0,temp1);
        premultiAlpha->GetOutput(&temp2);

        direct2d_device_context->SetTarget(direct2d_bitmap.get());

        direct2d_device_context->BeginDraw();
        direct2d_device_context->Clear();
        direct2d_device_context->DrawImage(temp2);
        hr = direct2d_device_context->EndDraw();
        if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET){
            MessageBoxA(GetForegroundWindow(),"Failed to Draw Effects to DXGI_SURFACE",NULL,MB_OK);
            Core::SafeRelease(&direct2d_device_context);
            recreateDeviceContext = true;
        }
        // Core::SafeRelease(&direct2d_bitmap);
        // Core::SafeRelease(&temp1);
        // Core::SafeRelease(&premultiAlpha);
        // Core::SafeRelease(&first_target);
        // delete imgData;
        // recreateDeviceContext = true;

    };

    void DXBDCompositionImageRenderTarget::commit(){
        DXGI_PRESENT_PARAMETERS params;
        params.DirtyRectsCount = NULL;
        params.pDirtyRects = NULL;
        params.pScrollOffset = NULL;
        params.pScrollRect = NULL;

        hr = dxgi_swap_chain_1->Present1(1,0,&params);
        if(FAILED(hr) || hr == DXGI_STATUS_OCCLUDED ){
            // Core::SafeRelease(&dxgi_swap_chain_1);
            //  Core::SafeRelease(&dxgi_swap_chain_2);
            Core::SafeRelease(&dxgi_surface);
            Core::SafeRelease(&direct2d_device_context);
            Core::SafeRelease(&direct2d_bitmap);
            recreateSwapChain = recreateDeviceContext = true;
        }
        else {
            recreateSwapChain = false;
            recreateDeviceContext = false;
            newTarget = false;
            // Core::SafeRelease(&direct2d_bitmap);
        }
        
        //    hr = dcomp_surface->EndDraw();
        //    if(FAILED(hr)){
        //         Core::SafeRelease(&direct2d_device_context);
        //         Core::SafeRelease(&direct2d_bitmap);
        //         Core::SafeRelease(&dcomp_surface);
        //         MessageBoxA(HWND_DESKTOP,"DComp Surface failed to End Draw","NOTE",MB_OK);
        //    }
        //    else {
        //        Core::SafeRelease(&dxgi_surface);
        //    };
            // direct2d_device_context->SetTarget(direct2d_bitmap.get());
            // direct2d_device_context->BeginDraw();
            // direct2d_device_context->DrawImage(first_target.get());
            // hr = direct2d_device_context->EndDraw();
            // if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET){
            //     Core::SafeRelease(&direct2d_device_context);
            //     Core::SafeRelease(&direct2d_bitmap);
            //     recreateDeviceContext = true;
            // }
            // else {
            //     hr = dcomp_surface->EndDraw();
            //     if(FAILED(hr) || hr == DCOMPOSITION_ERROR_SURFACE_NOT_BEING_RENDERED){
            //         Core::SafeRelease(&direct2d_device_context);
            //         Core::SafeRelease(&direct2d_bitmap);
            //     };
    };

    // Core::SharedPtr<BDCompositionImage> DXBDCompositionImageRenderTarget::getImg(){
    //     return nullptr;
    // };

    DXBDCompositionImageRenderTarget::~DXBDCompositionImageRenderTarget(){
         delete imgData;
         Core::SafeRelease(&first_target);
         Core::SafeRelease(&dxgi_surface);
        Core::SafeRelease(&direct2d_device_context);
        Core::SafeRelease(&direct2d_bitmap);
    };

};