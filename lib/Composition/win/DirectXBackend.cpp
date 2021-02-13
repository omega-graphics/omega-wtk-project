#include <dcomp.h>
#include <d3d12.h>
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
#pragma comment(lib, "d3d12.lib")
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
        rectres.right = (rectres.left + (rect.dimen.minWidth));

         rectres.bottom = ((parent_wnd_rect.bottom / sacleFactor) - rect.pos.y);
         rectres.top = rectres.bottom - (rect.dimen.minHeight);
        
        return rectres;
    };
    D2D1_POINT_2F core_position_to_d2_point(Core::Position & pos,HWND parent){
        #define DEFAULT_DPI 96.f
        UINT dpi = GetDpiForWindow(parent);

        FLOAT scaleFactor = FLOAT(dpi)/DEFAULT_DPI;

        RECT rc;
        GetClientRect(parent,&rc);
        auto wndHeight = rc.bottom - rc.top;

        return D2D1::Point2F(pos.x,(wndHeight / scaleFactor) - pos.y);

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

            bool has_direct2d_hwnd_target;
            UniqueComPtr<ID2D1HwndRenderTarget> direct2d_hwnd_target;
            bool has_dcomp_hwnd_target;
            UniqueComPtr<IDCompositionTarget> dcomp_hwnd_target;
            bool has_direct2d_layer;
            UniqueComPtr<ID2D1Layer> direct2d_layer;
            
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

            HWNDItemCompAssets():has_direct2d_hwnd_target(false),direct2d_hwnd_target(NULL),has_dcomp_hwnd_target(false),dcomp_hwnd_target(NULL),has_direct2d_layer(false),direct2d_layer(NULL){};

            ~HWNDItemCompAssets(){
                // releaseMap(solid_color_brushes);
                // releaseMap(text_formats);
            };
            
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

        bool match_size_u(D2D1_SIZE_U lhs,D2D1_SIZE_U rhs){
            return (lhs.height == rhs.height) && (lhs.width == rhs.width);
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
                compAssets->direct2d_hwnd_target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
                compAssets->has_direct2d_hwnd_target = true;
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

        void __setup_color_brush(UniqueComPtr<ID2D1HwndRenderTarget> & direct2d_hwnd_target,Core::Map<unsigned,HWNDItemCompAssets::SolidColorBrushEntry *> & solid_color_brushes,Color & color,unsigned id){
             HRESULT hr;
            auto it = solid_color_brushes.find(id);
            /// If the brush entry has not been created at all! (Initalization!)
            if(it == solid_color_brushes.end()){
                ID2D1SolidColorBrush *solidBrush;
                hr = direct2d_hwnd_target->CreateSolidColorBrush(color_to_d2d1_color(color),&solidBrush);
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
                        hr = direct2d_hwnd_target->CreateSolidColorBrush(new_color,&solidBrush);
                        if(!SUCCEEDED(hr)){
                            //Handle Error!
                        };
                        color_entry->setVal(solidBrush);
                    };
                }
                /// If the brush has been released due to a bad hwnd render target!
                else {
                    ID2D1SolidColorBrush *solidBrush;
                    hr = direct2d_hwnd_target->CreateSolidColorBrush(color_to_d2d1_color(color),&solidBrush);
                    if(!SUCCEEDED(hr)){
                    //Handle Error!
                    };
                    color_entry->setVal(solidBrush);
                };
            };
        };

        void setupSolidColorBrush(HWNDItemCompAssets *compAssets,Color &color,unsigned id,HWNDItemCompAssets::SolidColorBrushUsage usage){

            if(usage == HWNDItemCompAssets::SolidColorBrushUsage::Fill) {
                __setup_color_brush(compAssets->direct2d_hwnd_target,compAssets->fill_solid_color_brushes,color,id);
            }
            else if(usage == HWNDItemCompAssets::SolidColorBrushUsage::Border){
                __setup_color_brush(compAssets->direct2d_hwnd_target,compAssets->border_solid_color_brushes,color,id);
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
                hr = compAssets->direct2d_hwnd_target->CreateBitmap(D2D1::SizeU(bitmap.width,bitmap.height),bitmap.data,bitmap.stride,D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap_res);
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
                        hr = compAssets->direct2d_hwnd_target->CreateBitmap(D2D1::SizeU(bitmap.width,bitmap.height),bitmap.data,bitmap.stride,D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap_res);
                        if(!SUCCEEDED(hr)){
                            //Handle Error!
                        };
                        entry->setVal(bitmap_res);
                    };
                }
                else {
                    ID2D1Bitmap *bitmap_res;
                    hr = compAssets->direct2d_hwnd_target->CreateBitmap(D2D1::SizeU(bitmap.width,bitmap.height),bitmap.data,bitmap.stride,D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap_res);
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


        void discardAssets(HWNDItemCompAssets *assets){
            if(assets->has_direct2d_hwnd_target){
                Core::SafeRelease(&assets->direct2d_hwnd_target);
                assets->has_direct2d_hwnd_target = false;
            }
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
            ID2D1HwndRenderTarget * render_target = assets->direct2d_hwnd_target.get();
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
                        std::string info = "Width:" + std::to_string(params->img.width) + "Height:" + std::to_string(params->img.height) + "Stride:" + std::to_string(params->img.stride);
                        MessageBox(GetForegroundWindow(),info.c_str(),"NOTE",MB_OK);
                        setupBitmap(assets,params->img,visual->id);
                        ID2D1Bitmap *bitmap = assets->bitmaps[visual->id]->get();
                        RECT rc = core_rect_to_win_rect(params->rect,parent);
                        render_target->DrawBitmap(bitmap,D2D1::RectF(rc.left,rc.top,rc.right,rc.bottom));
                        break;
                    };
                }
        };

        void doWork(){
            auto & objects = currentLayer->getTargetVisuals();
            auto native_item = (Native::Win::HWNDItem *)currentLayer->getTargetNativePtr();
            HWNDItemCompAssets assets;
            setupTarget(native_item,&assets);
            HRESULT hr;
            auto & render_target = assets.direct2d_hwnd_target;

            render_target->BeginDraw();
            render_target->SetTransform(D2D1::IdentityMatrix());
            render_target->Clear(color_to_d2d1_color(currentLayer->getBackgroundColor()));

            // render_target->PushLayer(D2D1::LayerParameters(),layer.get());

            auto object_it = objects.begin();
            while(object_it != objects.end()) {
                auto & visual = *object_it;
                drawVisual(visual,&assets,native_item->getHandle());
                ++object_it;
            };

            // render_target->PopLayer();

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
                // setupLayer(&hwndAssets);
                // auto & layer = hwndAssets.direct2d_layer;

                render_target->BeginDraw();
                render_target->SetTransform(D2D1::IdentityMatrix());
                render_target->Clear(color_to_d2d1_color(currentLayer->getBackgroundColor()));

                // render_target->PushLayer(D2D1::LayerParameters(),layer.get());

                auto object_it = objects.begin();
                while(object_it != objects.end()) {
                    auto & visual = *object_it;
                    drawVisual(visual,&hwndAssets,hwndItem->getHandle());
                    ++object_it;
                };

                // render_target->PopLayer();

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