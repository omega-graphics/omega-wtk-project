#include "DXBDCompositionFontFactory.h"
#include <atlstr.h>

namespace OmegaWTK::Composition {

    void cpp_str_to_cpp_wstr(Core::String str,std::wstring & res){
        CStringW w_str(str.data());
        res = std::move(w_str);
    };
    DXBDCompositionFont::DXBDCompositionFont(IDWriteTextFormat *textFormat):textFormat(textFormat){};

    DXBDCompositionFont::~DXBDCompositionFont(){
        Core::SafeRelease(&textFormat);
    };

    DXBDCompositionFontFactory::DXBDCompositionFontFactory(IDWriteFactory *factory):dwrite_factory(factory){};

    Core::SharedPtr<BDCompositionFontFactory> DXBDCompositionFontFactory::Create(IDWriteFactory *factory){
        return std::make_shared<DXBDCompositionFontFactory>(factory);
    };

    Core::SharedPtr<BDCompositionFont> DXBDCompositionFontFactory::createFont(Text::Font &font, unsigned int fontSize){
        HRESULT hr;
        IDWriteTextFormat *textFormat;
        std::wstring w_str;
        cpp_str_to_cpp_wstr(font.family,w_str);

        DWRITE_FONT_WEIGHT weight;
        DWRITE_FONT_STYLE style;

        switch (font.style) {
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

        DWRITE_TEXT_ALIGNMENT hAlign;
        DWRITE_PARAGRAPH_ALIGNMENT vAlign;
        DWRITE_WORD_WRAPPING textWrap;

        switch (font.textAlignment) {
         /// Left Align!
            case Text::Font::LeftUpper : {
                hAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                break;
            };
            case Text::Font::LeftCenter : {
                hAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
                break;
            };
            case Text::Font::LeftLower : {
                hAlign = DWRITE_TEXT_ALIGNMENT_LEADING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
                break;
            };
            /// Middle Align!
            case Text::Font::MiddleUpper : {
                hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                break;
            };
            case Text::Font::MiddleCenter : {
                hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
                break;
            };
            case Text::Font::MiddleLower : {
                hAlign = DWRITE_TEXT_ALIGNMENT_CENTER;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
                break;
            };
            /// Right Align!
            case Text::Font::RightUpper : {
                hAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
                break;
            };
            case Text::Font::RightCenter : {
                hAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
                break;
            };
            case Text::Font::RightLower : {
                hAlign = DWRITE_TEXT_ALIGNMENT_TRAILING;
                vAlign = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
                break;
            };
        }

        switch (font.wrapping) {
            case Text::Font::None : {
                textWrap = DWRITE_WORD_WRAPPING_NO_WRAP;
                break;
            };
            case Text::Font::WrapByWord : {
                textWrap = DWRITE_WORD_WRAPPING_WRAP;
                break;
            };
            case Text::Font::WrapByCharacter : {
                textWrap = DWRITE_WORD_WRAPPING_CHARACTER;
                break;
            };
        }

        UINT dpi = GetDpiForWindow(GetForegroundWindow());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;

        /// TODO: Use Custom Fonts with custom font Collection!
        hr = dwrite_factory->CreateTextFormat(w_str.c_str(),NULL,weight,style,DWRITE_FONT_STRETCH_NORMAL,FLOAT(fontSize) * scaleFactor,L"en-us",&textFormat);
        if(FAILED(hr)){

        };
        textFormat->SetTextAlignment(hAlign);
        textFormat->SetParagraphAlignment(vAlign);
        textFormat->SetWordWrapping(textWrap);

        return std::make_shared<DXBDCompositionFont>(textFormat);
    };
    void DXBDCompositionFontFactory::destroyFont(Core::SharedPtr<BDCompositionFont> &font){

    };
};