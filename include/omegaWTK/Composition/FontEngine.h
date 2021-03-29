#include "omegaWTK/Core/Core.h"

#ifdef TARGET_WIN32
#include <dwrite.h>
#pragma comment(lib,"dwrite.lib")
#endif

#ifdef TARGET_MACOS
#import <CoreText/CoreText.h>
#endif

#ifndef OMEGAWTK_COMPOSITION_FONTENGINE_H
#define OMEGAWTK_COMPOSITION_FONTENGINE_H

namespace OmegaWTK {
class AppInst;
};

namespace OmegaWTK::Composition {

class Font;

class TextRect {
protected:
    Core::String text_val;
    Core::SharedPtr<Font> font;
    virtual void _updateStrInternal() = 0;
public:
    Core::Rect rect;
    virtual void *getNative() = 0;
    virtual void getGlyphBoundingBoxes(Core::Rect ** rects,unsigned * count) = 0;
    Core::String & getString() noexcept{ return text_val;};
    void setString(const Core::String & str){
        text_val = std::move(str);
        _updateStrInternal();
    };
    static Core::SharedPtr<TextRect> Create(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect rect);
protected:
    TextRect(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect & rect):text_val(_val),rect(rect),font(font){};
};

struct FontDescriptor {
    typedef enum : OPT_PARAM {
        Regular,
        Italic,
        Bold,
        BoldAndItalic
    } FontStyle;
    typedef enum : OPT_PARAM {
        LeftUpper,
        LeftCenter,
        LeftLower,
        MiddleUpper,
        MiddleCenter,
        MiddleLower,
        RightUpper,
        RightCenter,
        RightLower
    }Alignment;
    typedef enum : OPT_PARAM {
        None,
        WrapByWord,
        WrapByCharacter
    } Wrapping;
    Core::String family;
    FontStyle style;
    Alignment textAlignment;
    Wrapping wrapping;
    unsigned size;
    FontDescriptor(Core::String _family,unsigned size,FontStyle _style = Regular,Alignment textAlignment = LeftUpper,Wrapping wrapping = WrapByWord):family(_family),style(_style),textAlignment(textAlignment),wrapping(wrapping),size(size){};
    ~FontDescriptor(){};
};


class Font {
public:
    FontDescriptor desc;
    Font(FontDescriptor & desc):desc(desc){};
    virtual void *getNativeFont() = 0;
//    virtual void getBoundingBoxesFromTextBox(TextRect &textRect) = 0;
};

class FontEngine {
public:
    Core::SharedPtr<Font> CreateFont(FontDescriptor & desc);
    static FontEngine * const instance;
private:
    friend class ::OmegaWTK::AppInst;
    static void Create(){
        
    };
    static void Destroy(){
        delete instance;
    };
};

};

#endif
