#include <dcomp.h>
#include <d3d12.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <dwrite.h>

#include <wrl.h>

#undef DrawText

#include "DirectXBackend.h"
#include "NativePrivate/win/HWNDItem.h" 
#include "omegaWTK/Composition/Layer.h"

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dcomp.lib")
#pragma comment(lib, "dwrite.lib")

namespace OmegaWTK::Composition {

    void cpp_str_to_cpp_wstr(std::string & str,std::wstring & res){
        res.reserve(str.size());
        MultiByteToWideChar(CP_UTF8,MB_COMPOSITE,str.c_str(),str.size(),res.data(),str.size());
    };

    D2D1::ColorF color_to_d2d1_color(Color & color){
        #define EIGHTBIT_TO_FLOAT(num) (num/255.f)
        return D2D1::ColorF(EIGHTBIT_TO_FLOAT(color.r),EIGHTBIT_TO_FLOAT(color.g),EIGHTBIT_TO_FLOAT(color.b),EIGHTBIT_TO_FLOAT(color.a));
    };

    void core_rect_to_win_rect(Core::Rect &rect,LPRECT rectres){
        rectres->left = rect.pos.x;
        rectres->bottom = rect.pos.y;
        rectres->right = rect.pos.x + rect.dimen.minWidth;
        rectres->top = rect.pos.y + rect.dimen.minHeight;
    };
    /// A ComPtr that releases its object on its destruction. (Similar to the std::unique_ptr)
    template<class T>
    class UniqueComPtr {
    public:
        Microsoft::WRL::ComPtr<T> comPtr;
        T * get() { return comPtr.Get();};
        T * operator->(){
            return comPtr.Get();
        };
        T ** operator&(){
            return comPtr.GetAddressOf();
        };
        UniqueComPtr() = default;
        UniqueComPtr(Microsoft::WRL::ComPtr<T> _com):comPtr(_com){};
        UniqueComPtr(T *ptr):comPtr(ptr){};
        ~UniqueComPtr(){
            auto ptr = comPtr.Detach();
            Core::SafeRelease(&ptr);
        };
    };

    class DirectXBackend : public Backend {

        struct HWNDItemCompAssets {
            bool has_direct2d_hwnd_target;
            UniqueComPtr<ID2D1HwndRenderTarget> direct2d_hwnd_target;
            bool has_dcomp_hwnd_target;
            UniqueComPtr<IDCompositionTarget> dcomp_hwnd_target;
            bool has_direct2d_brush;
             UniqueComPtr<ID2D1Brush> brush;
        };

        using HWNDItemCompAssetsCatalog = Core::Map<Native::Win::HWNDItem *,HWNDItemCompAssets>;
        HWNDItemCompAssetsCatalog assets_catalog;

        UniqueComPtr<ID3D12Device1> direct3d_device;
        UniqueComPtr<ID2D1Factory1> direct2d_factory;
        UniqueComPtr<IDXGIDevice> dxgi_device;
        UniqueComPtr<ID2D1Device> direct2d_device;
        UniqueComPtr<IDCompositionDevice3> dcomp_device;
        UniqueComPtr<IDWriteFactory> dwrite_factory;

        void setup(){
            // MessageBox(GetForegroundWindow(),"Setting Up Backend","Note",MB_OK);
            HRESULT hr;
            hr = D3D12CreateDevice(NULL,D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&direct3d_device));
            if(!SUCCEEDED(hr)){
                //Handle Error!
            };
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,&direct2d_factory);
            if(!SUCCEEDED(hr)){
                //Handle Error!
            };
            hr = direct3d_device.comPtr.As(&dxgi_device.comPtr);
            if(!SUCCEEDED(hr)){
                //Handle Error!
            };
            hr = DCompositionCreateDevice3(dxgi_device.get(),IID_PPV_ARGS(&dcomp_device));
            if(!SUCCEEDED(hr)){
                //Handle Error!
            };
            hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED,__uuidof(IDWriteFactory),(IUnknown **)&dwrite_factory);
            if(!SUCCEEDED(hr)){
                //Handle Error!
            };

        };

        void setupTarget(Native::Win::HWNDItem *native_item,HWNDItemCompAssets *compAssets){
            HRESULT hr;
            if(!compAssets->has_direct2d_hwnd_target){
                ID2D1HwndRenderTarget *render_target;
                 RECT rc = native_item->getClientRect();
                auto target_size = D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top);
                hr = direct2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),D2D1::HwndRenderTargetProperties(native_item->getHandle(),target_size),&render_target);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };
                compAssets->direct2d_hwnd_target = render_target;
                compAssets->has_direct2d_hwnd_target = true;
            };
            // return S_OK;
        };

        void setupBrush(HWNDItemCompAssets *compAssets,Color &color){
            HRESULT hr;
            if(!compAssets->has_direct2d_brush){
                ID2D1SolidColorBrush *solidBrush;
                hr = compAssets->direct2d_hwnd_target->CreateSolidColorBrush(color_to_d2d1_color(color),&solidBrush);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };
                compAssets->has_direct2d_brush = true;
                compAssets->brush = solidBrush;
            };
            // return S_OK;
        };
        void discardAssets(HWNDItemCompAssets *assets){
            if(assets->has_direct2d_hwnd_target){
                Core::SafeRelease(&assets->direct2d_hwnd_target);
                assets->has_direct2d_hwnd_target = false;
            }
            if(assets->has_direct2d_brush){
                Core::SafeRelease(&assets->brush);
                assets->has_direct2d_brush = false;
            };
        };

        void drawVisual(Visual *visual,HWNDItemCompAssets *assets,RECT rc){

            HRESULT hr;
            ID2D1HwndRenderTarget * render_target = assets->direct2d_hwnd_target.get();
            switch (visual->type) {
                    // case Visual::Text : {
                    //    Visual::TextParams *params = (Visual::TextParams *)visual->params;
                    //    std::wstring wide_str;
                    //     ///Step 1: Convert std::string to std::wstring
                    //    cpp_str_to_cpp_wstr(params->str,wide_str);

                    //    IDWriteTextFormat * textFormat;
                    //    /// Step 2: Create Text Format to use!
                    //    hr = dwrite_factory->CreateTextFormat(L"Arial",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,params->size,L"UTF-8",&textFormat);
                    //    if(!SUCCEEDED(hr)){
                    //         //Handle Error!
                    //    };  
                    //    ID2D1SolidColorBrush *color_brush;
                    //    /// Step 3: Create Brush
                    //    hr = render_target->CreateSolidColorBrush(color_to_d2d1_color(params->textColor),&color_brush);
                    //    if(!SUCCEEDED(hr)){
                    //         //Handle Error!
                    //    };

                    //    /// Step 4: Draw the Text onto HWND.
                    //    render_target->DrawTextA(wide_str.c_str(),wide_str.size(),textFormat,D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),color_brush);
                    //    break;
                    // }
                    case Visual::Rect : {
                        Visual::RectParams *params = (Visual::RectParams *)visual->params;
                        /// Step 1: Create Brush
                        ID2D1SolidColorBrush *color_brush;
                        setupBrush(assets,params->color);
                        color_brush = (ID2D1SolidColorBrush *)assets->brush.get();
                        RECT rect_;
                        /// Step 2: Convert Core::Rect to RECT struct
                        core_rect_to_win_rect(params->rect,&rect_);
                        auto d2_rect = D2D1::RectF(rect_.left,rect_.top,rect_.right,rect_.bottom);
                        /// Step 3: Frame the Rectangle with Brush
                        // render_target->DrawRectangle(d2_rect,color_brush);
                        /// Step 4: Fill Rectangle with Brush.
                        render_target->FillRectangle(d2_rect,color_brush);
                        break;
                    }
                    case Visual::RoundedRect : {
                        Visual::RoundedRectParams *params = (Visual::RoundedRectParams *)visual->params;
                        /// Step 1: Create Brush
                        ID2D1SolidColorBrush *color_brush;
                        setupBrush(assets,params->color);
                        color_brush = (ID2D1SolidColorBrush *)assets->brush.get();
                        RECT rect_;
                        /// Step 2: Convert Core::Rect to RECT struct
                        core_rect_to_win_rect(params->rect,&rect_);
                        auto d2_rounded_rect = D2D1::RoundedRect(D2D1::RectF(rect_.left,rect_.top,rect_.right,rect_.bottom),params->rad_x,params->rad_y);
                        /// Step 3: Frame the Rectangle with Brush
                        render_target->DrawRoundedRectangle(d2_rounded_rect,color_brush);

                        /// Step 4: Fill Rectangle with Brush.
                        render_target->FillRoundedRectangle(d2_rounded_rect,color_brush);
                        break;
                    }
                }
        };

        void doWork(){
            auto & objects = currentLayer->getTargetVisuals();
            auto native_item = (Native::Win::HWNDItem *)currentLayer->getTargetNativePtr();
            HWNDItemCompAssets assets {false,NULL,false,NULL,false,NULL};
            setupTarget(native_item,&assets);
            HRESULT hr;
            auto & render_target = assets.direct2d_hwnd_target;

            render_target->BeginDraw();
            render_target->SetTransform(D2D1::IdentityMatrix());
            render_target->Clear(D2D1::ColorF(D2D1::ColorF::White));

            auto object_it = objects.begin();
            while(object_it != objects.end()) {
                auto & visual = *object_it;
                drawVisual(visual,&assets,native_item->getClientRect());
                ++object_it;
            };

            hr = render_target->EndDraw();
            if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
                discardAssets(&assets);
            }
            
            assets_catalog.insert(std::pair<Native::Win::HWNDItem *,HWNDItemCompAssets>(native_item,assets));
        
        };
        void doUpdate(){
            auto hwndItem = (Native::Win::HWNDItem *)currentLayer->getTargetNativePtr();
            auto & objects = currentLayer->getTargetVisuals();
            auto found = assets_catalog.find(hwndItem);
            HRESULT hr;
            if(found != assets_catalog.end()){
                auto & hwndAssets = found->second;
                setupTarget(hwndItem,&hwndAssets);
                auto & render_target = hwndAssets.direct2d_hwnd_target;

                render_target->BeginDraw();
                render_target->SetTransform(D2D1::IdentityMatrix());
                render_target->Clear(D2D1::ColorF(D2D1::ColorF::White));

                auto object_it = objects.begin();
                while(object_it != objects.end()) {
                    auto & visual = *object_it;
                    drawVisual(visual,&hwndAssets,hwndItem->getClientRect());
                    ++object_it;
                };
                hr = render_target->EndDraw();
                if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
                    discardAssets(&hwndAssets);
                }
            }
        };
    public:
        DirectXBackend(){
            setup();
        };
        ~DirectXBackend(){
            
        };
    };

    Backend * make_directx_backend(){
        return new DirectXBackend();
    };
}