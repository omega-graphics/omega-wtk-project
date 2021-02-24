#import "MTLBDCompositionFontFactory.h"

#import <Cocoa/Cocoa.h>

namespace OmegaWTK::Composition {

CFStringRef core_string_to_cf_string_ref(Core::String & str){
    return CFStringCreateWithCString(kCFAllocatorDefault,str.c_str(),CFStringGetSystemEncoding());
};

//CFDictionaryRef make_cf_dictionary(const void **keys,const void **vals,CFIndex num){
//    return CFDictionaryCreate(kCFAllocatorDefault,keys,vals,num,NULL,NULL);
//};

MTLBDCompositionFont::MTLBDCompositionFont(CTFontRef font, Text::Font & omegawtkFont):font(font),omegawtkFont(omegawtkFont){
    
};

Core::SharedPtr<BDCompositionFont> MTLBDCompositionFontFactory::createFont(Text::Font & font,unsigned fontSize){
    CTFontRef _font = CTFontCreateWithNameAndOptions(core_string_to_cf_string_ref(font.family),CGFloat(fontSize), NULL,kCTFontOptionsPreferSystemFont);
    CTFontSymbolicTraits fontTraits;
    
    switch (font.style) {
        case Text::Font::Bold:
            fontTraits = kCTFontTraitBold;
            break;
        case Text::Font::Italic:
            fontTraits = kCTFontTraitItalic;
            break;
        case Text::Font::BoldAndItalic:
            fontTraits = kCTFontTraitBold | kCTFontTraitItalic;
            break;
        case Text::Font::Regular :
            fontTraits = 0;
            break;
        default:
            break;
    }
    
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
    CTFontRef _font_final = CTFontCreateCopyWithSymbolicTraits(_font,CGFloat(fontSize * scaleFactor),NULL,kCTFontTraitBold | kCTFontTraitItalic, fontTraits);
    CFRelease(_font);
    
    return std::make_shared<MTLBDCompositionFont>(_font_final,font);
};

void MTLBDCompositionFontFactory::destroyFont(Core::SharedPtr<BDCompositionFont> &font){
    
};

MTLBDCompositionFontFactory::MTLBDCompositionFontFactory(){};

Core::SharedPtr<BDCompositionFontFactory> MTLBDCompositionFontFactory::Create(){
    return std::make_unique<MTLBDCompositionFontFactory>();
};




};
