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
        return D2D1::ColorF(color.r,color.g,color.b,color.a);
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

        void doWork(){
            auto & tasks = currentLayer->getTargetTasks();
            auto native_item = (Native::Win::HWNDItem *)currentLayer->getTargetNativePtr();
            HRESULT hr;
            ID2D1HwndRenderTarget *render_target;

            RECT rc = native_item->getClientRect();

            hr = direct2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),D2D1::HwndRenderTargetProperties(native_item->getHandle()),&render_target);
                if(!SUCCEEDED(hr)){
                //Handle Error!
            };

            render_target->BeginDraw();

            while(!tasks.empty()) {
                auto task = tasks.front();
                tasks.pop();
                switch (task->type) {
                    case Task::DrawText : {
                       Task::DrawTextParams *params = (Task::DrawTextParams *)task->params;
                       std::wstring wide_str;
                        ///Step 1: Convert std::string to std::wstring
                       cpp_str_to_cpp_wstr(params->str,wide_str);

                       IDWriteTextFormat * textFormat;
                       /// Step 2: Create Text Format to use!
                       hr = dwrite_factory->CreateTextFormat(L"Arial",NULL,DWRITE_FONT_WEIGHT_NORMAL,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,params->size,L"UTF-8",&textFormat);
                       if(!SUCCEEDED(hr)){
                            //Handle Error!
                       };  
                       ID2D1SolidColorBrush *color_brush;
                       /// Step 3: Create Brush
                       hr = render_target->CreateSolidColorBrush(color_to_d2d1_color(params->textColor),&color_brush);
                       if(!SUCCEEDED(hr)){
                            //Handle Error!
                       };

                       /// Step 4: Draw the Text onto HWND.
                       render_target->DrawTextA(wide_str.c_str(),wide_str.size(),textFormat,D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom),color_brush);
                       break;
                    }
                    case Task::DrawRect : {
                        Task::DrawRectParams *params = (Task::DrawRectParams *)task->params;
                        ID2D1SolidColorBrush *color_brush;
                        hr = render_target->CreateSolidColorBrush(color_to_d2d1_color(params->color),&color_brush);
                        if(!SUCCEEDED(hr)){
                            //Handle Error!
                        };
                        RECT rect_;
                        core_rect_to_win_rect(params->rect,&rect_);

                        render_target->DrawRectangle(D2D1::RectF(rect_.left,rect_.top,rect_.right,rect_.bottom),color_brush);
                        break;
                    }
                    case Task::DrawRoundedRect : {
                        Task::DrawRoundedRectParams *params = (Task::DrawRoundedRectParams *)task->params;
                        ID2D1SolidColorBrush *color_brush;
                        hr = render_target->CreateSolidColorBrush(color_to_d2d1_color(params->color),&color_brush);
                        if(!SUCCEEDED(hr)){
                            //Handle Error!
                        };
                        RECT rect_;
                        core_rect_to_win_rect(params->rect,&rect_);
                        render_target->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect_.left,rect_.top,rect_.right,rect_.bottom),params->rad_x,params->rad_y),color_brush);
                        break;
                    }
                }
            };

            render_target->EndDraw();
            
            
        };
    public:
        DirectXBackend(){
            setup();
        };
        ~DirectXBackend(){
            
        };
    };

    Backend * make_directx_backend(){
        return (Backend *)new DirectXBackend();
    };
}