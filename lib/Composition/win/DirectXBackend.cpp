#include <dcomp.h>
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <dwrite.h>
#include <UIAnimation.h>
#include <Uxtheme.h>
#include <atlstr.h>
#include <wrl.h>
#include <iomanip>
#include <sstream>

#undef DrawText

#include "DirectXBackend.h"
#include "NativePrivate/win/HWNDItem.h" 
#include "omegaWTK/Composition/Layer.h"

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dcomp.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "uxtheme.lib")


namespace OmegaWTK::Composition {

    void cpp_str_to_cpp_wstr(Core::String str,std::wstring & res){
        CString st (str.data());
        CStringW sw (st);
        res = std::move(sw);
    };

    // void cpp_wstr_to_cpp_str(std::wstring &str,Core::String & res){
    //     CStringW sw(str.data());
    //     CString st (sw);
    //     res = std::move(st);
    // };

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
    D2D1_POINT_2F core_position_to_d2_point(Core::Position & pos,HWND parent){
        #define DEFAULT_DPI 96.f
        UINT dpi = GetDpiForWindow(parent);

        FLOAT scaleFactor = FLOAT(dpi)/DEFAULT_DPI;

        RECT rc;
        GetClientRect(parent,&rc);
        auto wndHeight = rc.bottom - rc.top;

        return D2D1::Point2F(pos.x,(wndHeight) - pos.y);

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
        // UniqueComPtr(Microsoft::WRL::ComPtr<T> _com):comPtr(_com){};
        UniqueComPtr(T *ptr):comPtr(ptr){};
        ~UniqueComPtr(){
            auto ptr = comPtr.Detach();
            Core::SafeRelease(&ptr);
        };
    };

    class DirectXBackend : public Backend {
        /// The Assets for a single HWNDItem!
        class HWNDItemCompAssets {
            template<class _Ty>
            struct Entry {
            private:
                bool hasEntry;
                UniqueComPtr<_Ty> ptr;
            public:
                bool & hasVal(){ return hasEntry;};
                void setVal(_Ty * _ptr){
                    hasEntry = true;
                    ptr = _ptr;
                };
                void releaseVal(){
                    Core::SafeRelease(&ptr);
                    hasEntry = false;
                };
                _Ty *get(){
                    return ptr.get();
                };
                Entry(_Ty *ptr):hasEntry(true),ptr(ptr){};
            };

            public:

            using SolidColorBrushEntry = Entry<ID2D1SolidColorBrush>;
            using TextFormatEntry = Entry<IDWriteTextFormat>;
            using BitmapEntry = Entry<ID2D1Bitmap>;

            bool has_dxgi_swap_chain;
            UniqueComPtr<IDXGISwapChain1> dxgi_chain;
            bool has_dxgi_surface;
            UniqueComPtr<IDXGISurface> dxgi_surface;
            bool has_direct2d_bitmap;
            UniqueComPtr<ID2D1Bitmap1> d2_bitmap;
            bool has_direct2d_device_context;
            UniqueComPtr<ID2D1DeviceContext> direct2d_device_context;
            bool has_dcomp_hwnd_target;
            UniqueComPtr<IDCompositionTarget> dcomp_hwnd_target;
            
            template<typename _Ent_Ty>
            using MapEntry = std::pair<unsigned,_Ent_Ty>;

            enum class SolidColorBrushUsage : OPT_PARAM {
                Fill,
                Border
            };

            Core::Map<unsigned,SolidColorBrushEntry *> fill_solid_color_brushes;
            Core::Map<unsigned,SolidColorBrushEntry *> border_solid_color_brushes;
            Core::Map<unsigned,TextFormatEntry *> text_formats;
            Core::Map<unsigned,BitmapEntry *> bitmaps;
            
            template<class _Ty>
            void releaseMap(Core::Map<unsigned,_Ty> & core_map){
                auto it = core_map.begin();
                while(it != core_map.end()){
                    auto &entry = it->second;
                    entry->releaseVal();
                    delete entry;
                    ++it;
                };
            };

            HWNDItemCompAssets():has_dxgi_swap_chain(false),dxgi_chain(NULL),has_dxgi_surface(false),dxgi_surface(NULL),has_dcomp_hwnd_target(false),dcomp_hwnd_target(NULL){};

            ~HWNDItemCompAssets(){
                // releaseMap(solid_color_brushes);
                // releaseMap(text_formats);
            };
            
        };

        using HWNDItemCompAssetsCatalog = Core::Map<Native::Win::HWNDItem *,HWNDItemCompAssets>;
        HWNDItemCompAssetsCatalog assets_catalog;

        UniqueComPtr<ID3D11Device> direct3d_device;
        // UniqueComPtr<ID3D11DeviceContext> direct3d_device_context;
        // D3D_FEATURE_LEVEL level[10];
        UniqueComPtr<ID2D1Factory1> direct2d_factory;
        UniqueComPtr<IDXGIDevice1> dxgi_device;
        UniqueComPtr<ID2D1Device> direct2d_device;
        UniqueComPtr<IDXGIAdapter> dxgi_adapter;
        UniqueComPtr<IDXGIFactory2> dxgi_factory;
        UniqueComPtr<IDCompositionDevice3> dcomp_device;
        UniqueComPtr<IDWriteFactory> dwrite_factory;

        void setup(){
            MessageBox(GetForegroundWindow(),"Setting Up Backend","Note",MB_OK);
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
            hr = D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,D3D11_CREATE_DEVICE_BGRA_SUPPORT,feature_level,ARRAYSIZE(feature_level),D3D11_SDK_VERSION,&direct3d_device,NULL,NULL);
            if(!SUCCEEDED(hr)){
                //Handle Error!
                std::ostringstream out;
                out << std::hex << hr;

                MessageBox(GetForegroundWindow(),(std::string("Failed to Direct3D Create Device!") + out.str()).c_str(),"Note",MB_OK);
            };
            hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,&direct2d_factory);
            if(!SUCCEEDED(hr)){
                //Handle Error!
            };
            hr = direct3d_device->QueryInterface(IID_PPV_ARGS(&dxgi_device));
            if(!SUCCEEDED(hr)){
                //Handle Error!
                MessageBoxA(GetForegroundWindow(),"Failed to Get DXGI Device!",NULL,MB_OK);
            };
            hr = dxgi_device->GetAdapter(&dxgi_adapter);
            if(!SUCCEEDED(hr)){
                MessageBoxA(GetForegroundWindow(),"Failed to Get DXGI Adapter",NULL,MB_OK);
            }
            hr = dxgi_device->SetMaximumFrameLatency(1);
            if(!SUCCEEDED(hr)){
                MessageBoxA(GetForegroundWindow(),"Failed to Set Frame Latency!",NULL,MB_OK);
            }

            hr = dxgi_adapter->GetParent(IID_PPV_ARGS(&dxgi_factory));
            if(!SUCCEEDED(hr)){
                MessageBoxA(GetForegroundWindow(),"Failed to Get DXGI Factory!",NULL,MB_OK);
            }
            hr = direct2d_factory->CreateDevice(dxgi_device.get(),&direct2d_device);
            if(!SUCCEEDED(hr)){
                MessageBoxA(GetForegroundWindow(),"Failed to Create D2Device ",NULL,MB_OK);
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

        bool match_size_u(D2D1_SIZE_U lhs,D2D1_SIZE_U rhs){
            return (lhs.height == rhs.height) && (lhs.width == rhs.width);
        };

        void setupTarget(Native::Win::HWNDItem *native_item,HWNDItemCompAssets *compAssets){
            HRESULT hr;
            if(!compAssets->has_dxgi_swap_chain){
                DXGI_SWAP_CHAIN_DESC1 desc {0};
                RECT rc = native_item->getClientRect();

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

                
                IDXGISwapChain1 *swap_chain;
                hr = dxgi_factory->CreateSwapChainForHwnd(direct3d_device.get(),native_item->getHandle(),&desc,NULL,NULL,&swap_chain);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                    std::ostringstream out;
                    out << std::hex << hr;
                    MessageBoxA(GetForegroundWindow(),(std::string("Failed to Create Swap Chain! CODE:") + out.str()).c_str(),NULL,MB_OK);
                };
                ID2D1DeviceContext *context;
                hr = direct2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&context);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                     MessageBoxA(GetForegroundWindow(),"Failed to Create Device Context!",NULL,MB_OK);
                };
                
                // compAssets->direct2d_hwnd_target = render_target;
                // compAssets->direct2d_hwnd_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
                // compAssets->has_direct2d_hwnd_target = true;
                IDXGISurface *surface;
                hr = swap_chain->GetBuffer(0,IID_PPV_ARGS(&surface));
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };

                UINT dpi = GetDpiForWindow(native_item->getHandle());
                
                ID2D1Bitmap1 *bitmap;
                hr = context->CreateBitmapFromDxgiSurface(surface,D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_CANNOT_DRAW | D2D1_BITMAP_OPTIONS_TARGET,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED),dpi,dpi),&bitmap);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };
                compAssets->direct2d_device_context = context;
                compAssets->direct2d_device_context->SetTarget(bitmap);
                compAssets->has_direct2d_device_context = true;

                compAssets->dxgi_chain = swap_chain;
                compAssets->has_dxgi_swap_chain = true;

                compAssets->dxgi_surface = surface;
                compAssets->has_dxgi_surface = true;

                compAssets->d2_bitmap = bitmap;
                compAssets->has_direct2d_bitmap = true;
            }
        };
        // void setupLayer(HWNDItemCompAssets *assets){
        //     HRESULT hr;
        //     if(!assets->has_direct2d_layer){
        //         ID2D1Layer *layer;
        //         hr = assets->direct2d_hwnd_target->CreateLayer(NULL,&layer);
        //         if(!SUCCEEDED(hr)){
        //             //Handle Error!
        //         };
        //         assets->direct2d_layer = layer;
        //         assets->has_direct2d_layer = true;
        //     };
        // };
        /// Checks to see if the two colors inputted are equalivent!
        bool compareD2D1Colors(const D2D1_COLOR_F & first,const D2D1_COLOR_F & second){
            return  (first.r == second.r) && (first.g == second.g) && (first.b == second.b) && (first.a == second.a);
        };

        void __setup_color_brush(UniqueComPtr<ID2D1DeviceContext> & device_context,Core::Map<unsigned,HWNDItemCompAssets::SolidColorBrushEntry *> & solid_color_brushes,Color & color,unsigned id){
             HRESULT hr;
            auto it = solid_color_brushes.find(id);
            /// If the brush entry has not been created at all! (Initalization!)
            if(it == solid_color_brushes.end()){
                ID2D1SolidColorBrush *solidBrush;
                hr = device_context->CreateSolidColorBrush(color_to_d2d1_color(color),&solidBrush);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };
                
                solid_color_brushes.insert(HWNDItemCompAssets::MapEntry<HWNDItemCompAssets::SolidColorBrushEntry *>(id,new HWNDItemCompAssets::SolidColorBrushEntry(solidBrush)));
            }
            
            else  {
                auto & color_entry = it->second;
                if(color_entry->hasVal()) {
                /// If the brush exists but the color is not the same!
                    auto new_color = color_to_d2d1_color(color);
                    if(!compareD2D1Colors(color_entry->get()->GetColor(),new_color)) {
                        color_entry->releaseVal();
                        ID2D1SolidColorBrush *solidBrush;
                        hr = device_context->CreateSolidColorBrush(new_color,&solidBrush);
                        if(!SUCCEEDED(hr)){
                            //Handle Error!
                        };
                        color_entry->setVal(solidBrush);
                    };
                }
                /// If the brush has been released due to a bad hwnd render target!
                else {
                    ID2D1SolidColorBrush *solidBrush;
                    hr = device_context->CreateSolidColorBrush(color_to_d2d1_color(color),&solidBrush);
                    if(!SUCCEEDED(hr)){
                    //Handle Error!
                    };
                    color_entry->setVal(solidBrush);
                };
            };
        };

        void setupSolidColorBrush(HWNDItemCompAssets *compAssets,Color &color,unsigned id,HWNDItemCompAssets::SolidColorBrushUsage usage){

            if(usage == HWNDItemCompAssets::SolidColorBrushUsage::Fill) {
                __setup_color_brush(compAssets->direct2d_device_context,compAssets->fill_solid_color_brushes,color,id);
            }
            else if(usage == HWNDItemCompAssets::SolidColorBrushUsage::Border){
                __setup_color_brush(compAssets->direct2d_device_context,compAssets->border_solid_color_brushes,color,id);
            };
            // return S_OK;
        };

        DXGI_FORMAT computePixelFormat(unsigned bitDepth,unsigned channelCount){
            std::string message = "BitDepth :" + std::to_string(bitDepth) + "ChannelCount:" + std::to_string(channelCount);
            MessageBox(GetForegroundWindow(),message.c_str(),"NOTE",MB_OK);
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
                        return  DXGI_FORMAT_R8G8B8A8_TYPELESS;
                        break;
                    };
                }
            };
            // return DXGI_FORMAT_UNKNOWN;
        };

        void setupBitmap(HWNDItemCompAssets *compAssets,Core::BitmapImage &bitmap,unsigned v_id){
            HRESULT hr;
            auto it = compAssets->bitmaps.find(v_id);
            if(it == compAssets->bitmaps.end()){
                ID2D1Bitmap *bitmap_res;
                hr = compAssets->direct2d_device_context->CreateBitmap(D2D1::SizeU(bitmap.width,bitmap.height),bitmap.data,bitmap.stride,D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap_res);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                    std::stringstream s;
                    s << "Failed to Create Bitmap: Error Code:" << std::hex << hr;
                    MessageBox(GetForegroundWindow(),s.str().c_str(),NULL,MB_OK);
                };
                compAssets->bitmaps.insert(std::pair<unsigned,HWNDItemCompAssets::BitmapEntry *>(v_id,new HWNDItemCompAssets::BitmapEntry(bitmap_res)));
            }
            else {
                auto & entry = it->second;
                if(entry->hasVal()){
                    auto size = entry->get()->GetSize();
                    /// If image has changed!
                    if(!(size.height == bitmap.height && size.width == bitmap.width)){
                        entry->releaseVal();
                        ID2D1Bitmap *bitmap_res;
                        hr = compAssets->direct2d_device_context->CreateBitmap(D2D1::SizeU(bitmap.width,bitmap.height),bitmap.data,bitmap.stride,D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap_res);
                        if(!SUCCEEDED(hr)){
                            //Handle Error!
                        };
                        entry->setVal(bitmap_res);
                    };
                }
                else {
                    ID2D1Bitmap *bitmap_res;
                    hr = compAssets->direct2d_device_context->CreateBitmap(D2D1::SizeU(bitmap.width,bitmap.height),bitmap.data,bitmap.stride,D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap_res);
                    if(!SUCCEEDED(hr)){
                        //Handle Error!
                    };
                    entry->setVal(bitmap_res);
                };
            }
        };

        void setupTextFormatIfNeeded(HWNDItemCompAssets *compAssets,Text & text,unsigned id){
            HRESULT hr;
            auto it = compAssets->text_formats.find(id);
            if(it == compAssets->text_formats.end()){
                std::wstring fontName;
                cpp_str_to_cpp_wstr(text.getFont().family,fontName);


                DWRITE_FONT_WEIGHT weight;
                DWRITE_FONT_STYLE style;

                switch (text.getFont().style) {
                    case Text::Font::BoldAndItalic : {
                        style = DWRITE_FONT_STYLE_ITALIC;
                        weight = DWRITE_FONT_WEIGHT_BOLD;
                        break;
                    }
                    case Text::Font::Bold : {
                        weight = DWRITE_FONT_WEIGHT_BOLD;
                        style = DWRITE_FONT_STYLE_NORMAL;
                        break;
                    }
                    case Text::Font::Italic : {
                        weight = DWRITE_FONT_WEIGHT_NORMAL;
                        style = DWRITE_FONT_STYLE_ITALIC;
                        break;
                    }
                    case Text::Font::Regular : {
                        weight = DWRITE_FONT_WEIGHT_NORMAL;
                        style = DWRITE_FONT_STYLE_NORMAL;
                        break;
                    };
                }
                IDWriteTextFormat *textFormat;

                hr = dwrite_factory->CreateTextFormat(fontName.c_str(),NULL,weight,style,DWRITE_FONT_STRETCH_NORMAL,text.getFontSize(),L"en-us",&textFormat);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };

                hr = textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };
                hr = textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
                if(!SUCCEEDED(hr)){
                    //Handle Error!
                };
                // const DWRITE_TRIMMING trimmingParams = {DWRITE_TRIMMING_GRANULARITY_WORD,'\n',1};
                // hr = textFormat->SetTrimming(&trimmingParams,NULL);
                // if(!SUCCEEDED(hr)){
                //     //Handle Error!
                // };

                compAssets->text_formats.insert(HWNDItemCompAssets::MapEntry<HWNDItemCompAssets::TextFormatEntry *>(id,new HWNDItemCompAssets::TextFormatEntry(textFormat)));
            } /// If text_format entry exists but new_format might be different!
            // else {
            //     auto & entry = it->second;
            //     auto txt_format = entry->get();
            //     auto & font = text.getFont();

            //     bool needsChange = false;

            //     UINT32 w_buffer_len = txt_format->GetFontFamilyNameLength();
            //     WCHAR w_buffer[w_buffer_len];
            //     hr = txt_format->GetFontFamilyName(w_buffer,w_buffer_len);

            //     if(!SUCCEEDED(hr)){
            //         //Handle Error!
            //     };

            //     std::wstring newFontName;
            //     cpp_str_to_cpp_wstr(font.family,newFontName);

            //     if(newFontName != w_buffer)
            //         needsChange = true;

            //     auto weight = txt_format->GetFontWeight();
            //     auto style = txt_format->GetFontStyle();

            //     switch (font.style) {
            //         case Text::Font::Regular : {
            //             if(weight != DWRITE_FONT_WEIGHT_NORMAL) {
            //                 weight = DWRITE_FONT_WEIGHT_NORMAL; 
            //                 needsChange = true;
            //             }
            //             if(style != DWRITE_FONT_STYLE_NORMAL) {
            //                 style = DWRITE_FONT_STYLE_NORMAL;
            //                 needsChange = true;
            //             }
            //             break;
            //         };
            //         case Text::Font::Italic : {
            //             if(weight != DWRITE_FONT_WEIGHT_NORMAL) {
            //                 weight = DWRITE_FONT_WEIGHT_NORMAL;
            //                 needsChange = true;
            //             }
            //             if(style != DWRITE_FONT_STYLE_ITALIC) {
            //                 style = DWRITE_FONT_STYLE_ITALIC;
            //                 needsChange = true;
            //             }
            //             break;
            //         };
            //         case Text::Font::Bold : {
            //             if(weight != DWRITE_FONT_WEIGHT_BOLD) {
            //                 weight = DWRITE_FONT_WEIGHT_BOLD;
            //                 needsChange = true;
            //             }
            //             if(style != DWRITE_FONT_STYLE_NORMAL) {
            //                 style = DWRITE_FONT_STYLE_NORMAL;
            //                 needsChange = true;
            //             }
            //             break;
            //         };
            //         case Text::Font::BoldAndItalic : {
            //             if(weight != DWRITE_FONT_WEIGHT_BOLD) {
            //                 weight = DWRITE_FONT_WEIGHT_BOLD;
            //                 needsChange = true;
            //             }
            //             if(style != DWRITE_FONT_STYLE_ITALIC) {
            //                 style = DWRITE_FONT_STYLE_ITALIC;
            //                 needsChange = true;
            //             }
            //             break;
            //         };
            //     }

            //     auto fontSize = txt_format->GetFontSize();
            //     if(fontSize != FLOAT(text.getFontSize())){
            //         fontSize = text.getFontSize();
            //         needsChange = true;
            //     };

            //     if(needsChange){
            //         entry->releaseVal();
            //         IDWriteTextFormat  *text_format;
            //         hr = dwrite_factory->CreateTextFormat(w_buffer,NULL,weight, style,DWRITE_FONT_STRETCH_NORMAL, fontSize,L"en-us",&text_format);

            //         if(!SUCCEEDED(hr)){
            //         //Handle Error!
            //         };

            //         entry->setVal(text_format);
            //     };
            // }
        };
        template<class _Ty>
        void __release_map(_Ty & map){
            auto map_it =  map.begin();
            while(map_it != map.end()){
                auto & entry = map_it->second;
                if(entry->hasVal())
                    entry->releaseVal();
                ++map_it;
            };
        };


        void discardAssets(HWNDItemCompAssets *assets,bool context_only = true){
            if(!context_only){

                if(assets->has_dxgi_swap_chain){
                    Core::SafeRelease(&assets->dxgi_chain);
                    assets->has_dxgi_swap_chain = false;
                }
                if(assets->has_dxgi_surface){
                    Core::SafeRelease(&assets->dxgi_surface);
                    assets->has_dxgi_surface = false;
                };
                if(assets->has_direct2d_bitmap){
                    Core::SafeRelease(&assets->d2_bitmap);
                    assets->has_direct2d_bitmap = false;
                };

            };
            if(assets->has_direct2d_device_context){
                Core::SafeRelease(&assets->direct2d_device_context);
                assets->has_direct2d_device_context = false;
            };
            // if(assets->has_direct2d_layer){
            //     Core::SafeRelease(&assets->direct2d_layer);
            //     assets->has_direct2d_layer = false;
            // };
            /// Release Brushes!
            __release_map(assets->fill_solid_color_brushes);
            __release_map(assets->border_solid_color_brushes);
            __release_map(assets->text_formats);

        };

        void drawVisual(Visual *visual,HWNDItemCompAssets *assets,HWND parent){

            // HRESULT hr;
            ID2D1DeviceContext * render_target = assets->direct2d_device_context.get();
            switch (visual->type) {
                    case Visual::Text : {
                       Visual::TextParams *params = (Visual::TextParams *)visual->params;
                       IDWriteTextFormat *textFormat;
                       setupTextFormatIfNeeded(assets,params->text,visual->id);
                        textFormat = assets->text_formats[visual->id]->get();

                       
                       setupSolidColorBrush(assets,params->color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Fill);
                       ID2D1SolidColorBrush *color_brush = assets->fill_solid_color_brushes[visual->id]->get();

                       std::wstring text_to_render;
                        cpp_str_to_cpp_wstr(params->text.getString(),text_to_render);
                        RECT text_cont = core_rect_to_win_rect(params->rect,parent);

                       /// Step 4: Draw the Text onto HWND.
                       render_target->DrawTextA(text_to_render.c_str(),text_to_render.size(),textFormat,D2D1::RectF(text_cont.left,text_cont.top,text_cont.right,text_cont.bottom),color_brush);
                       break;
                    }
                    case Visual::Rect : {
                        Visual::RectParams *params = (Visual::RectParams *)visual->params;
                        /// Step 1: Create Brush

                        setupSolidColorBrush(assets,params->color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Fill);
                        ID2D1SolidColorBrush *color_brush = assets->fill_solid_color_brushes[visual->id]->get();

                        RECT rect_ = core_rect_to_win_rect(params->rect,parent);
                        /// Step 2: Convert Core::Rect to RECT struct
                        // RECT rc;
                        // GetClientRect(parent,&rc);

                        auto d2_rect = D2D1::RectF(rect_.left,rect_.top,rect_.right,rect_.bottom);
                        // std::string s1 = "RECT for Red Rectangle: L:" + std::to_string(rect_.left) + ",T:" + std::to_string(rect_.top) + ",R:" + std::to_string(rect_.right) + ",B:" + std::to_string(rect_.bottom);

                        RECT rc;
                        GetClientRect(parent,&rc);
                        // std::string s2 = "RECT for Parent Window: L:" + std::to_string(rc.left) + ",T:" + std::to_string(rc.top) + ",R:" + std::to_string(rc.right) + ",B:" + std::to_string(rc.bottom);
                        // MessageBox(GetForegroundWindow(),s2.c_str(),"NOTE",MB_OK);
                        // MessageBox(GetForegroundWindow(),s1.c_str(),"NOTE",MB_OK);
                        /// Step 3: Frame the Rectangle with Border Brush
                        /// Step 4: Fill Rectangle with Fill Brush.
                        render_target->FillRectangle(d2_rect,color_brush);
                        if(params->border.has_value()) {
                            auto & border_obj = params->border.value();
                            setupSolidColorBrush(assets,border_obj.color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Border);
                            ID2D1SolidColorBrush *border_color_brush = assets->border_solid_color_brushes[visual->id]->get();

                            render_target->DrawRectangle(d2_rect,border_color_brush,border_obj.width);
                        }
                        break;
                    }
                    case Visual::Ellipse : {
                        // MessageBox(GetForegroundWindow(),"Setting Up Ellipse:","Note:",MB_OK);
                        Visual::EllipseParams *params = (Visual::EllipseParams *)visual->params;
                        setupSolidColorBrush(assets,params->color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Fill);
                        ID2D1SolidColorBrush *color_brush = assets->fill_solid_color_brushes[visual->id]->get();

                        UINT dpi = GetDpiForWindow(parent);
                        // MessageBox(GetForegroundWindow(),(std::string("DPI:") + std::to_string(dpi)).c_str(),"Note:",MB_OK);

                        auto d2_ellipse = D2D1::Ellipse(core_position_to_d2_point(params->ellipse.pos,parent),params->ellipse.radius_x,params->ellipse.radius_y);
                        
                        render_target->FillEllipse(d2_ellipse,color_brush);
                        if(params->border.has_value()) {
                            auto & border_obj = params->border.value();
                            setupSolidColorBrush(assets,border_obj.color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Border);
                            ID2D1SolidColorBrush *border_color_brush = assets->border_solid_color_brushes[visual->id]->get();
                            render_target->DrawEllipse(d2_ellipse,border_color_brush,border_obj.width);
                        }
                        break;
                    };
                    case Visual::RoundedRect : {
                        Visual::RoundedRectParams *params = (Visual::RoundedRectParams *)visual->params;
                        /// Step 1: Create Brush
                       
                        setupSolidColorBrush(assets,params->color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Fill);
                         ID2D1SolidColorBrush *color_brush = assets->fill_solid_color_brushes[visual->id]->get();
                        RECT rect_ = core_rect_to_win_rect(params->rect,parent);

                        // UINT dpi = GetDpiForWindow(parent);

                        // FLOAT scaleFactor = FLOAT(dpi)/96.f;
                        /// Step 2: Convert Core::Rect to RECT struct
                        ;
                        auto d2_rounded_rect = D2D1::RoundedRect(D2D1::RectF(rect_.left,rect_.top,rect_.right,rect_.bottom),params->rad_x,params->rad_y);
                        /// Step 3: Frame the Rectangle with Border Brush
                      

                        /// Step 4: Fill Rectangle with Fill Brush.
                        render_target->FillRoundedRectangle(d2_rounded_rect,color_brush);
                        if(params->border.has_value()) {
                            auto & border_obj = params->border.value();
                            setupSolidColorBrush(assets,border_obj.color,visual->id,HWNDItemCompAssets::SolidColorBrushUsage::Border);
                            ID2D1SolidColorBrush *border_color_brush = assets->border_solid_color_brushes[visual->id]->get();
                            render_target->DrawRoundedRectangle(d2_rounded_rect,border_color_brush,border_obj.width);
                        }
                        break;
                    }
                    case Visual::Bitmap : {
                        Visual::BitmapParams *params = (Visual::BitmapParams *)visual->params;
                        // std::string info = "Width:" + std::to_string(params->img.width) + "Height:" + std::to_string(params->img.height) + "Stride:" + std::to_string(params->img.stride);
                        // MessageBox(GetForegroundWindow(),info.c_str(),"NOTE",MB_OK);
                        setupBitmap(assets,params->img,visual->id);
                        ID2D1Bitmap *bitmap = assets->bitmaps[visual->id]->get();
                        RECT rc = core_rect_to_win_rect(params->rect,parent);
                        render_target->DrawBitmap(bitmap,D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom));
                        break;
                    };
                }
        };

        void doWork(){
             DXGI_PRESENT_PARAMETERS params;
            params.DirtyRectsCount = 0;
            params.pDirtyRects = NULL;
            params.pScrollOffset = NULL;
            params.pScrollRect = NULL;

            auto & objects = currentLayer->getTargetVisuals();
            auto native_item = (Native::Win::HWNDItem *)currentLayer->getTargetNativePtr();
            HWNDItemCompAssets assets;
            setupTarget(native_item,&assets);
            HRESULT hr;
            // auto & render_target = assets.direct2d_hwnd_target;
            auto & device_context = assets.direct2d_device_context;
            auto & swap_chain = assets.dxgi_chain;

            device_context->BeginDraw();
            device_context->SetTransform(D2D1::IdentityMatrix());
            device_context->Clear(color_to_d2d1_color(currentLayer->getBackgroundColor()));


            auto object_it = objects.begin();
            while(object_it != objects.end()) {
                auto & visual = *object_it;
                drawVisual(visual,&assets,native_item->getHandle());
                ++object_it;
            };

            hr = device_context->EndDraw();
            /// If Device Context Failed to Draw to DXGISurface, ditch the direct2d_context only!
            if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
                discardAssets(&assets,true);
            }
            else {
                hr = swap_chain->Present1(1,0,&params);
                /// IF Presenting the DXGISurface fails, ditch all assets!
                if(FAILED(hr) || hr == DXGI_STATUS_OCCLUDED ){
                    discardAssets(&assets,false);
                };
            }
            
            assets_catalog.insert(std::pair<Native::Win::HWNDItem *,HWNDItemCompAssets>(native_item,assets));
        
        };
        void doUpdate(){
             DXGI_PRESENT_PARAMETERS params;
            params.DirtyRectsCount = NULL;
            params.pDirtyRects = NULL;
            params.pScrollOffset = NULL;
            params.pScrollRect = NULL;

            auto hwndItem = (Native::Win::HWNDItem *)currentLayer->getTargetNativePtr();
            auto & objects = currentLayer->getTargetVisuals();
            auto found = assets_catalog.find(hwndItem);
            HRESULT hr;
            if(found != assets_catalog.end()){
                auto & hwndAssets = found->second;
                setupTarget(hwndItem,&hwndAssets);
                auto & swap_chain = hwndAssets.dxgi_chain;
                auto & device_context  = hwndAssets.direct2d_device_context;
                // setupLayer(&hwndAssets);
                // auto & layer = hwndAssets.direct2d_layer;

                device_context->BeginDraw();
                device_context->SetTransform(D2D1::IdentityMatrix());
                device_context->Clear(color_to_d2d1_color(currentLayer->getBackgroundColor()));

                // render_target->PushLayer(D2D1::LayerParameters(),layer.get());

                auto object_it = objects.begin();
                while(object_it != objects.end()) {
                    auto & visual = *object_it;
                    drawVisual(visual,&hwndAssets,hwndItem->getHandle());
                    ++object_it;
                };

                // render_target->PopLayer();

                hr = device_context->EndDraw();
                if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET) {
                    discardAssets(&hwndAssets,true);
                }
                else {
                    hr = swap_chain->Present1(1,0,&params);
                    /// IF Presenting the DXGISurface fails, ditch all assets!
                    if(FAILED(hr) || hr == DXGI_STATUS_OCCLUDED ){
                        discardAssets(&hwndAssets,false);
                    };
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