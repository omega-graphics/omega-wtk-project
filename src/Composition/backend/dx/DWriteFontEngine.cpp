#include "omegaWTK/Composition/FontEngine.h"
#include <atlstr.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1helper.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <d3d12.h>
#include <dcommon.h>
#include <dwrite.h>
#include <dxgi.h>
#include <dxgiformat.h>

#pragma comment(lib,"dxguid.lib")

namespace OmegaWTK::Composition {

    class FontEnumerator : public IDWriteFontFileEnumerator {
        IDWriteFactory * dwrite_factory;
        unsigned refCount = 1;
        std::wstring font_file;
    public:
        explicit FontEnumerator(OmegaCommon::StrRefBase<wchar_t> font_file):font_file(font_file){

        };
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject) override {
            if(iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator)){
                AddRef();
                *ppvObject = this;
                return S_OK;
            }
            else {
                *ppvObject = nullptr;
                return E_NOINTERFACE;
            }
        };
        ULONG STDMETHODCALLTYPE AddRef() override {
            return InterlockedIncrement(&refCount);
        };
        ULONG STDMETHODCALLTYPE Release() override{
            ULONG n = InterlockedDecrement(&refCount);
            if(n == 0){
                delete this;
            }
            return n;
        };
        explicit FontEnumerator(IDWriteFactory * dwrite_factory):dwrite_factory(dwrite_factory){

        }
        HRESULT MoveNext(BOOL *hasCurrentFile) noexcept override {
            return S_OK;
        }
        HRESULT GetCurrentFontFile(IDWriteFontFile **fontFile) noexcept override {
            HRESULT hr = dwrite_factory->CreateFontFileReference(font_file.c_str(),nullptr,fontFile);
            return hr;
        }
    };

/// @note The Collection Key is the font file path to be loaded.
    class FontLoader : public IDWriteFontCollectionLoader {
        unsigned refCount = 1;
    public:
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject) override{
            if(iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader)){
                AddRef();
                *ppvObject = this;
                return S_OK;
            }
            else {
                *ppvObject = nullptr;
                return E_NOINTERFACE;
            }
        };
        ULONG STDMETHODCALLTYPE AddRef() override{
            return InterlockedIncrement(&refCount);
        };
        ULONG STDMETHODCALLTYPE Release() override{
            ULONG n = InterlockedDecrement(&refCount);
            if(n == 0){
                delete this;
            }
            return n;
        };
        HRESULT CreateEnumeratorFromKey(IDWriteFactory *factory, const void *collectionKey, UINT32 collectionKeySize, IDWriteFontFileEnumerator **fontFileEnumerator) noexcept override {
            OmegaCommon::StrRefBase<wchar_t> path((wchar_t *)collectionKey,collectionKeySize);
            auto ptr = new FontEnumerator(path);
            *fontFileEnumerator = ptr;
            return S_OK;
        }
    };

    void cpp_str_to_cpp_wstr(OmegaCommon::String str,std::basic_string<wchar_t> & res){
        res.resize(str.size());
        int32_t len;
        UErrorCode err;
        u_strFromUTF8((UChar *)res.data(),res.size(),&len,str.c_str(),str.size(),&err);

    };

     FontEngine * FontEngine::instance = nullptr;
     SharedHandle<FontLoader> font_loader;

     void FontEngine::Create(){
         instance = new FontEngine();
     };

     FontEngine * FontEngine::inst(){
         return instance;
     }

     void FontEngine::Destroy(){
         delete instance;
     };

     FontEngine::~FontEngine(){
         dwrite_factory->UnregisterFontCollectionLoader(font_loader.get());
         font_loader.reset();
     }

     FontEngine::FontEngine(){

         font_loader = std::make_shared<FontLoader>();
        
         HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),(IUnknown **)&dwrite_factory);
         if(FAILED(hr)){
             exit(1);
         };

         D3D_FEATURE_LEVEL levels[] = {D3D_FEATURE_LEVEL_11_0};
         auto d3d12_dev = (ID3D12Device *)gte.graphicsEngine->underlyingNativeDevice();

         D3D12_COMMAND_QUEUE_DESC desc {};
         desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
         desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
         desc.NodeMask = d3d12_dev->GetNodeCount();
         desc.Priority = 0;
         hr = d3d12_dev->CreateCommandQueue(&desc,IID_PPV_ARGS(&d3d11_on_12_queue));

         ID3D11Device *dev;

         hr = D3D11On12CreateDevice(d3d12_dev,D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,levels,1,(IUnknown *const *)&d3d11_on_12_queue,1,d3d12_dev->GetNodeCount(),(ID3D11Device **)&dev,&d3d11_devicecontext,nullptr);
         if(FAILED(hr)){
             OMEGAWTK_DEBUG("Failed to create D3D11On12");
             exit(1);
         }
         dev->QueryInterface(&d3d11_device);
         
         IDXGIDevice *dxgi_dev;

         dev->QueryInterface(IID_PPV_ARGS(&dxgi_dev));
         if(FAILED(hr)){
             OMEGAWTK_DEBUG("Failed to Query DXGI Device from D3D11on12Device");
            exit(1);
         }

         hr = D2D1CreateDevice(dxgi_dev,D2D1::CreationProperties(D2D1_THREADING_MODE_SINGLE_THREADED,D2D1_DEBUG_LEVEL_WARNING,D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS),&d2d1device);
        if(FAILED(hr)){
            OMEGAWTK_DEBUG("Failed to Create D2D1 Device");
            exit(1);
        }
         dwrite_factory->RegisterFontCollectionLoader(font_loader.get());
     };

     class DWriteFont : public Font {
     public:
         Core::UniqueComPtr<IDWriteTextFormat> textFormat;
         DWriteFont(FontDescriptor & desc,IDWriteTextFormat *textFormat):Font(desc),textFormat(textFormat){};
         void * getNativeFont(){
             return (void *)textFormat.get();
         };
         ~DWriteFont(){
             Core::SafeRelease(&textFormat);
         };
     };

     class DWriteGlyphRun : public GlyphRun {
     public:
         Core::UniqueComPtr<IDWriteTextLayout> textLayout;
         explicit DWriteGlyphRun(const OmegaWTK::UniString & str, Core::SharedPtr<Font> &font){
             auto *_font = (DWriteFont *)font.get();
            FontEngine::instance->dwrite_factory->CreateTextLayout((WCHAR *)str.getBuffer(),str.length(),_font->textFormat.get(),0,0,&textLayout);
         }
         Core::Rect getBoundingRectOfGlyphAtIndex(size_t glyphIdx) override {
            return Core::Rect {{0.f,0.f},0,0};
         }
     };

    Core::SharedPtr<GlyphRun>
    GlyphRun::fromUStringAndFont(const OmegaWTK::UniString &str, Core::SharedPtr<Font> &font) {
        return Core::SharedPtr<GlyphRun>(new DWriteGlyphRun(str,font));
    }

     class DWriteTextRect : public TextRect {
        DWRITE_PARAGRAPH_ALIGNMENT paraAlignment;
        DWRITE_TEXT_ALIGNMENT textAlignment;
        DWRITE_WORD_WRAPPING wrapping;
        DWRITE_FLOW_DIRECTION flowDirection;
        OmegaGTE::SharedHandle<OmegaGTE::GETexture> target;
        OmegaGTE::SharedHandle<OmegaGTE::GEFence> fence;
        ID3D11Texture2D *resource;
        IDXGISurface *surface;
        ID2D1DeviceContext *context;
     public:
         BitmapRes toBitmap() override {
             return {target,fence};
         };
         void drawRun(Core::SharedPtr<GlyphRun> &glyphRun,const Composition::Color &color) override {
             auto run = std::dynamic_pointer_cast<DWriteGlyphRun>(glyphRun);
            //  UINT dpi = GetDpiFromDpiAwarenessContext(GetThreadDpiAwarenessContext());
            //  FLOAT scaleFactor = FLOAT(dpi)/96.f;
             run->textLayout->SetMaxWidth(rect.w);
             run->textLayout->SetMaxHeight(rect.h);
             run->textLayout->SetParagraphAlignment(paraAlignment);
             run->textLayout->SetFlowDirection(flowDirection);
             run->textLayout->SetTextAlignment(textAlignment);
             run->textLayout->SetWordWrapping(wrapping);

               FontEngine::inst()->d3d11_device->AcquireWrappedResources((ID3D11Resource *const *)&resource,1);

             context->BeginDraw();
             context->Clear(D2D1::ColorF(1.f,0.f,0.f,1.f));
             ID2D1Brush * brush;
             context->CreateSolidColorBrush(D2D1::ColorF(color.r,color.g,color.b,color.a),(ID2D1SolidColorBrush **)&brush);
             context->DrawTextLayout(D2D1::Point2F(0,0),run->textLayout.get(),brush,D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT);
             HRESULT hr = context->EndDraw();

              FontEngine::instance->d3d11_device->ReleaseWrappedResources((ID3D11Resource *const *)&resource,1);
            
             FontEngine::inst()->d3d11_devicecontext->Flush();

            auto native_fence = (ID3D12Fence *)fence->native();

            FontEngine::inst()->d3d11_on_12_queue->Signal(native_fence,1);

             Core::SafeRelease(&brush);
             if(hr == D2DERR_RECREATE_TARGET){
                 Core::SafeRelease(&context);
                 D2D1CreateDeviceContext(surface,D2D1::CreationProperties(D2D1_THREADING_MODE_MULTI_THREADED,D2D1_DEBUG_LEVEL_WARNING,D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS),&context);
             }
         }
         void * getNative() override{
            return nullptr;
         };
         explicit DWriteTextRect(Core::Rect & rect,const TextLayoutDescriptor & layoutDesc): TextRect(rect),target(nullptr),context(nullptr){
             HRESULT hr;

             switch (layoutDesc.alignment) {
                case TextLayoutDescriptor::LeftUpper : {
                    textAlignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
                    paraAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                    break;
                }
             }

             wrapping = DWRITE_WORD_WRAPPING_NO_WRAP;

             OmegaGTE::TextureDescriptor textureDesc {OmegaGTE::GETexture::Texture2D};
             textureDesc.usage = OmegaGTE::GETexture::RenderTarget;
             textureDesc.height = (unsigned)rect.h;
             textureDesc.width = (unsigned)rect.w;             

             target = gte.graphicsEngine->makeTexture(textureDesc);
             
             D3D11_RESOURCE_FLAGS fgs{};
             fgs.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
             fgs.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
             fgs.CPUAccessFlags = D3D11_USAGE_DEFAULT;
             fgs.StructureByteStride = 0.f;

             OMEGAWTK_DEBUG("Ok! 1");

             hr = FontEngine::inst()->d3d11_device->CreateWrappedResource((IUnknown *)target->native(),&fgs,D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,IID_PPV_ARGS(&resource));
             if(FAILED(hr)){
                 OMEGAWTK_DEBUG("Failed to Create Wrapped Resource. ERR:" << std::hex << hr << std::dec);
                 exit(1);
             }

              OMEGAWTK_DEBUG("Ok! 2");
             hr = resource->QueryInterface(IID_PPV_ARGS(&surface));
             if(FAILED(hr)){
                  OMEGAWTK_DEBUG("Failed to get DXGISurface. ERR:" << std::hex << hr << std::dec);
                 exit(1);
             }

             OMEGAWTK_DEBUG("Ok! 3");
             hr = FontEngine::inst()->d2d1device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,&context);
             if(FAILED(hr)){
                  OMEGAWTK_DEBUG("Failed to create D2D1DeviceContext ERR:" << std::hex << hr << std::dec);
                 exit(1);
             }

             OMEGAWTK_DEBUG("Ok! 4");

             ID2D1Bitmap1 *bitmap;
             
            hr = context->CreateBitmapFromDxgiSurface(surface,D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET,D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM,D2D1_ALPHA_MODE_PREMULTIPLIED)),&bitmap);
             
            if(FAILED(hr)){
                  OMEGAWTK_DEBUG("Failed to create Bitmap from DXGISurface ERR:" << std::hex << hr << std::dec);
                 exit(1);
             }
             context->SetTarget(bitmap);

             OMEGAWTK_DEBUG("Ok! 5");

             context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
              OMEGAWTK_DEBUG("DWriteTextRect Successfully Created");

              fence = gte.graphicsEngine->makeFence();
         };
         ~DWriteTextRect() override {
             Core::SafeRelease(&surface);
            Core::SafeRelease(&context);
         }
     };

     SharedHandle<TextRect> TextRect::Create(Core::Rect rect,const TextLayoutDescriptor & layoutDesc){
         return SharedHandle<TextRect>(new DWriteTextRect(rect,layoutDesc));
     };

     Core::SharedPtr<Font> FontEngine::CreateFont(FontDescriptor &desc){
         HRESULT hr;
         IDWriteTextFormat *textFormat;
         std::wstring w_str;
         cpp_str_to_cpp_wstr(desc.family,w_str);

         DWRITE_FONT_WEIGHT weight;
         DWRITE_FONT_STYLE style;

         switch (desc.style) {
             case FontDescriptor::BoldAndItalic : {
                 style = DWRITE_FONT_STYLE_ITALIC;
                 weight = DWRITE_FONT_WEIGHT_BOLD;
                 break;
             }
             case FontDescriptor::Bold : {
                 weight = DWRITE_FONT_WEIGHT_BOLD;
                 style = DWRITE_FONT_STYLE_NORMAL;
                 break;
             }
             case FontDescriptor::Italic : {
                 weight = DWRITE_FONT_WEIGHT_NORMAL;
                 style = DWRITE_FONT_STYLE_ITALIC;
                 break;
             }
             case FontDescriptor::Regular : {
                 weight = DWRITE_FONT_WEIGHT_NORMAL;
                 style = DWRITE_FONT_STYLE_NORMAL;
                 break;
             };
         }

         UINT dpi = GetDpiForWindow(GetForegroundWindow());
         FLOAT scaleFactor = FLOAT(dpi)/96.f;

         /// TODO: Use Custom Fonts with custom font Collection!
         hr = dwrite_factory->CreateTextFormat(w_str.c_str(),NULL,weight,style,DWRITE_FONT_STRETCH_NORMAL,FLOAT(desc.size) * scaleFactor,L"en-us",&textFormat);
         if(FAILED(hr)){

         };


         return std::make_shared<DWriteFont>(desc,textFormat);
     };


    Core::SharedPtr<Font> FontEngine::CreateFontFromFile(OmegaCommon::FS::Path path, FontDescriptor &desc) {
        auto path_utf8 = path.str();
        UChar *str = new UChar[path_utf8.length()];
        int32_t len;
        UErrorCode err;
        u_strFromUTF8(str,path_utf8.length(),&len,path_utf8.c_str(),path_utf8.length(),&err);
        IDWriteFontCollection *collection;
        dwrite_factory->CreateCustomFontCollection(font_loader.get(),str,len,&collection);


        HRESULT hr;
        IDWriteTextFormat *textFormat;
        std::wstring w_str;
        cpp_str_to_cpp_wstr(desc.family,w_str);

        DWRITE_FONT_WEIGHT weight;
        DWRITE_FONT_STYLE style;

        switch (desc.style) {
            case FontDescriptor::BoldAndItalic : {
                style = DWRITE_FONT_STYLE_ITALIC;
                weight = DWRITE_FONT_WEIGHT_BOLD;
                break;
            }
            case FontDescriptor::Bold : {
                weight = DWRITE_FONT_WEIGHT_BOLD;
                style = DWRITE_FONT_STYLE_NORMAL;
                break;
            }
            case FontDescriptor::Italic : {
                weight = DWRITE_FONT_WEIGHT_NORMAL;
                style = DWRITE_FONT_STYLE_ITALIC;
                break;
            }
            case FontDescriptor::Regular : {
                weight = DWRITE_FONT_WEIGHT_NORMAL;
                style = DWRITE_FONT_STYLE_NORMAL;
                break;
            };
        }

        UINT dpi = GetDpiForWindow(GetForegroundWindow());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;

        /// TODO: Use Custom Fonts with custom font Collection!
        hr = dwrite_factory->CreateTextFormat(w_str.c_str(),collection,weight,style,DWRITE_FONT_STRETCH_NORMAL,FLOAT(desc.size) * scaleFactor,L"en-us",&textFormat);
        if(FAILED(hr)){

        };


        return std::make_shared<DWriteFont>(desc,textFormat);
    }

};