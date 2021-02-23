#include "../BDCompositionFontFactory.h"
#include "MTLBDCompositionDevice.h"

#import <CoreText/CoreText.h>

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONFONTFACTORY_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONFONTFACTORY_H

namespace OmegaWTK::Composition {

class MTLBDCompositionFont : public BDCompositionFont {
    CTFontRef font;
    Text::Font omegawtkFont;
    friend class MTLBDCompositionRenderTarget;
public:
    MTLBDCompositionFont(CTFontRef font,Text::Font & omegawtkFont);
};

class MTLBDCompositionFontFactory : public BDCompositionFontFactory {
public:
    MTLBDCompositionFontFactory();
    Core::SharedPtr<BDCompositionFont> createFont(Text::Font &font, unsigned fontSize);
    void destroyFont(Core::SharedPtr<BDCompositionFont> &font);
    static Core::SharedPtr<BDCompositionFontFactory> Create();
};

CFStringRef core_string_to_cf_string_ref(Core::String & str);

};

#endif
