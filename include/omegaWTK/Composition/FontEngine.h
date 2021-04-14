#include "omegaWTK/Core/Core.h"

#ifdef TARGET_WIN32
#include <dwrite.h>
#undef CreateFont
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
/**
 @brief A rectangular container that holds text drawn with a Font created by the FontEngine.
 @paragraph Description
 When creating an instance of this class, invoke the static method `Create` method, 
 which will return an instance of the appropriate platform specific subclass implementing the platform specific 
 features and bindings.   
*/
class OMEGAWTK_EXPORT  TextRect {
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
    /**
     @brief Creates a TextRect from a String, Font, Rect
     @param _val[in] The text's string.
     @param font[in] The Font used to draw the text with.
     @param rect[in] The Rect to draw the text in.
     @returns SharedPtr<TextRect>
    */
    static Core::SharedPtr<TextRect> Create(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect rect);
protected:
    TextRect(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect & rect):text_val(_val),rect(rect),font(font){};
};
/**
 @brief A struct that describes a Font that can be created by the FontEngine.
*/
struct OMEGAWTK_EXPORT  FontDescriptor {
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

/**
 @brief A Font created by the global instance of the FontEngine class. 
 Can be used in almost any context.
*/
class OMEGAWTK_EXPORT  Font {
public:
    FontDescriptor desc;
    Font(FontDescriptor & desc):desc(desc){};
    virtual void *getNativeFont() = 0;
//    virtual void getBoundingBoxesFromTextBox(TextRect &textRect) = 0;
};
/**
 @brief Font creation engine for Application
 @paragraph Description
  On application startup, a global instance of this class is created and ONLY one may exist throughout the entirety of the app's runtime. 
  This is mainly due to preventing the spawning of multiple IDWriteFactory instances (on Windows).
*/
class OMEGAWTK_EXPORT  FontEngine {
#ifdef TARGET_WIN32
    Core::UniqueComPtr<IDWriteFactory> dwrite_factory;
    friend class DWriteTextRect;
#endif
public:
    /**
     @brief Create a Font.
     @param desc[in] The Font Descriptor describing a Font.
     @paragraph Description
     Creates a shared instance of the font based on the FontDescriptor provided, 
     which can be used global context or local context such as a within the context of a CanvasView.
     @returns SharedPtr<Font>
    */
    Core::SharedPtr<Font> CreateFont(FontDescriptor & desc);
    Core::SharedPtr<Font> CreateFontFromFile(FSPath path,FontDescriptor & desc);
    static FontEngine * instance;
    /// @name Duplication Prevention Method Rules
    /// @{
    FontEngine(FontEngine &&) = delete;
    FontEngine(const FontEngine &) = delete;
    FontEngine operator=(FontEngine &&) = delete;
    /// @}
private:
    FontEngine();
    friend class ::OmegaWTK::AppInst;
    static void Create(){
        instance = new FontEngine();
    };
    static void Destroy(){
        delete instance;
    };
};

};

#endif
