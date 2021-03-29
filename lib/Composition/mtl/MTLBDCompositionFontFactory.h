#include "../BDCompositionFontFactory.h"
#include "MTLBDCompositionDevice.h"

#import <CoreText/CoreText.h>

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONFONTFACTORY_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCOMPOSITIONFONTFACTORY_H

namespace OmegaWTK::Composition {

template<class _Ty>
class MTLBDCompositionRenderTarget;

//class MTLBDCompositionFont : public BDCompositionFont {
//    CTFontRef font;
////    Text::Font omegawtkFont;
//    /// View Render Target
//    friend class MTLBDCompositionRenderTarget<BDCompositionViewRenderTarget>;
//    /// Image Render Target
//    friend class MTLBDCompositionRenderTarget<BDCompositionImageRenderTarget>;
//public:
//    MTLBDCompositionFont(CTFontRef font,Text::Font & omegawtkFont);
//};
//
//class MTLBDCompositionFontFactory : public BDCompositionFontFactory {
//public:
//    MTLBDCompositionFontFactory();
//    Core::SharedPtr<BDCompositionFont> createFont(Text::Font &font, unsigned fontSize);
//    void destroyFont(Core::SharedPtr<BDCompositionFont> &font);
//    static Core::SharedPtr<BDCompositionFontFactory> Create();
//};

CFStringRef core_string_to_cf_string_ref(Core::String & str);

};

#endif
