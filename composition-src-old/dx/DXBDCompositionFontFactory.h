#include "../BDCompositionFontFactory.h"
#include "DXBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONFONTFACTORY_H
#define OMEGAWTK_COMPOSITION_DX_DXBDCOMPOSITIONFONTFACTORY_H

namespace OmegaWTK::Composition {
    // class DXBDCompositionFont : public BDCompositionFont {
    // public:
    //     Core::UniqueComPtr<IDWriteTextFormat> textFormat;
    //     DXBDCompositionFont(IDWriteTextFormat *textFormat);
    //     // Text::Font::Alignment getTextAlignment();
    //     // Text::Font::Wrapping getTextWrapping();
    //     ~DXBDCompositionFont();
    // };

    // class DXBDCompositionFontFactory : public BDCompositionFontFactory {
    //     Core::UniqueComPtr<IDWriteFactory> dwrite_factory;
    // public:
    //     DXBDCompositionFontFactory(IDWriteFactory *factory);
    //     Core::SharedPtr<BDCompositionFont> createFont(Text::Font &font, unsigned fontSize);
    //     void destroyFont(Core::SharedPtr<BDCompositionFont> &font);
    //     static Core::SharedPtr<BDCompositionFontFactory> Create(IDWriteFactory *factory);
    // };

    // void cpp_str_to_cpp_wstr(Core::String str,std::wstring & res);
};

#endif