#include "omegaWTK/Composition/FontEngine.h"
#include <atlstr.h>

namespace OmegaWTK::Composition {

    void cpp_str_to_cpp_wstr(Core::String str,std::wstring & res){
        CStringW w_str(str.data());
        res = std::move(w_str);
    };

    FontEngine * const FontEngine::instance = new FontEngine();

    FontEngine::FontEngine(){

        HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),(IUnknown **)&dwrite_factory);
        if(FAILED(hr)){
            exit(1);
        };

    };

    class DWriteFont : public Font {
        Core::UniqueComPtr<IDWriteTextFormat> textFormat;
        friend class DWriteTextRect;
    public:
        DWriteFont(FontDescriptor & desc,IDWriteTextFormat *textFormat):Font(desc),textFormat(textFormat){};
        void * getNativeFont(){
            return (void *)textFormat.get();
        };
        ~DWriteFont(){
            Core::SafeRelease(&textFormat);
        };
    };

    class DWriteTextRect : public TextRect {
        Core::UniqueComPtr<IDWriteTextLayout> textLayout;
        void _updateStrInternal(){

        };
        Core::String string;
    public:
        void * getNative(){
            return (void *)textLayout.get();
        };
        DWriteTextRect(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect & rect):TextRect(_val,font,rect){
            HRESULT hr;
            std::wstring textVal;
            DWriteFont *dwrite_font = (DWriteFont *)font.get();
            cpp_str_to_cpp_wstr(_val,textVal);
            hr = FontEngine::instance->dwrite_factory->CreateTextLayout(textVal.c_str(),textVal.size(),dwrite_font->textFormat.get(),rect.dimen.minWidth,rect.dimen.minHeight,&textLayout);
            if(FAILED(hr)){
                // MessageBox!!
                exit(1);
            };

        };
        void getGlyphBoundingBoxes(Core::Rect **rects, unsigned int *count){
            HRESULT hr;
            IDWriteFontCollection *f_collection;
            WCHAR *familyName;
            hr = textLayout->GetFontCollection(&f_collection);
            hr = textLayout->GetFontFamilyName(familyName,textLayout->GetFontFamilyNameLength());
            UINT32 fontIdx;
            BOOL exists;
            hr = f_collection->FindFamilyName(familyName,&fontIdx,&exists);
            IDWriteFontFamily *fontFamily;
            hr = f_collection->GetFontFamily(fontIdx,&fontFamily);
            DWriteFont *dwrite_font = (DWriteFont *)font.get();
            IDWriteFontList *fontList;
            hr = fontFamily->GetMatchingFonts(dwrite_font->textFormat->GetFontWeight(),dwrite_font->textFormat->GetFontStretch(),dwrite_font->textFormat->GetFontStyle(),&fontList);\
            IDWriteFont *f;
            hr = fontList->GetFont(0,&f);
            DWRITE_FONT_METRICS f_metrics;
            f->GetMetrics(&f_metrics);
            
        };
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

        DWRITE_TEXT_ALIGNMENT hAlign;
        DWRITE_PARAGRAPH_ALIGNMENT vAlign;
        DWRITE_WORD_WRAPPING textWrap;

        switch (desc.textAlignment) {
         /// Left Align!
            case FontDescriptor::LeftUpper : {
                hAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                break;
            };
            case FontDescriptor::LeftCenter : {
                hAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
                break;
            };
            case FontDescriptor::LeftLower : {
                hAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
                break;
            };
            /// Middle Align!
            case FontDescriptor::MiddleUpper : {
                hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                break;
            };
            case FontDescriptor::MiddleCenter : {
                hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
                break;
            };
            case FontDescriptor::MiddleLower : {
                hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
                break;
            };
            /// Right Align!
            case FontDescriptor::RightUpper : {
                hAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                break;
            };
            case FontDescriptor::RightCenter : {
                hAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
                break;
            };
            case FontDescriptor::RightLower : {
                hAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
                break;
            };
        }

        switch (desc.wrapping) {
            case FontDescriptor::None : {
                textWrap = DWRITE_WORD_WRAPPING_NO_WRAP;
                break;
            };
            case FontDescriptor::WrapByWord : {
                textWrap = DWRITE_WORD_WRAPPING_WRAP;
                break;
            };
            case FontDescriptor::WrapByCharacter : {
                textWrap = DWRITE_WORD_WRAPPING_CHARACTER;
                break;
            };
        }

        UINT dpi = GetDpiForWindow(GetForegroundWindow());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;

        /// TODO: Use Custom Fonts with custom font Collection!
        hr = dwrite_factory->CreateTextFormat(w_str.c_str(),NULL,weight,style,DWRITE_FONT_STRETCH_NORMAL,FLOAT(desc.size) * scaleFactor,L"en-us",&textFormat);
        if(FAILED(hr)){

        };
        textFormat->SetTextAlignment(hAlign);
        textFormat->SetParagraphAlignment(vAlign);
        textFormat->SetWordWrapping(textWrap);

        return std::make_shared<DWriteFont>(desc,textFormat);
    };

};