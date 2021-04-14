#include "omegaWTK/Composition/FontEngine.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import <CoreText/CoreText.h>
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

namespace OmegaWTK::Composition {

CFStringRef core_string_to_cf_string_ref(Core::String & str){
    return CFStringCreateWithCString(kCFAllocatorDefault,str.c_str(),CFStringGetSystemEncoding());
};

FontEngine * FontEngine::instance;

FontEngine::FontEngine(){
    
};

class CoreTextFont : public Font {
    CTFontRef native;
    friend class CTTextRect;
public:
    CoreTextFont(FontDescriptor & desc,CTFontRef ref):Font(desc),native(ref){};
    void * getNativeFont(){
        return (void *)native;
    };
};

class CTTextRect : public TextRect {
    CTFramesetterRef framesetterRef;
    CTFrameRef frame;
    NSAttributedString *strData;
    void _updateStrInternal(){
        
    };
public:
    CTTextRect(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect & rect):TextRect(_val,font,rect){
        NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
        
        /// Initalize Text with Settings!
        [paragraphStyle setParagraphStyle:[NSParagraphStyle defaultParagraphStyle]];
        NSTextAlignment hAlignment;
        NSTextBlockVerticalAlignment vAlignment;
        switch (font->desc.textAlignment) {
            case FontDescriptor::LeftUpper: {
                hAlignment = NSTextAlignmentLeft;
                vAlignment = NSTextBlockTopAlignment;
                break;
            }
            case FontDescriptor::LeftCenter : {
                hAlignment = NSTextAlignmentLeft;
                vAlignment = NSTextBlockMiddleAlignment;
                break;
            };
            case FontDescriptor::LeftLower : {
                hAlignment = NSTextAlignmentLeft;
                vAlignment = NSTextBlockBottomAlignment;
                break;
            };
            case FontDescriptor::MiddleUpper: {
                hAlignment = NSTextAlignmentCenter;
                vAlignment = NSTextBlockTopAlignment;
                break;
            }
            case FontDescriptor::MiddleCenter : {
                hAlignment = NSTextAlignmentCenter;
                vAlignment = NSTextBlockMiddleAlignment;
                break;
            };
            case FontDescriptor::MiddleLower : {
                hAlignment = NSTextAlignmentCenter;
                vAlignment = NSTextBlockBottomAlignment;
                break;
            };
            case FontDescriptor::RightUpper: {
                hAlignment = NSTextAlignmentRight;
                vAlignment = NSTextBlockTopAlignment;
                break;
            }
            case FontDescriptor::RightCenter : {
                hAlignment = NSTextAlignmentRight;
                vAlignment = NSTextBlockMiddleAlignment;
                break;
            };
            case FontDescriptor::RightLower : {
                hAlignment = NSTextAlignmentRight;
                vAlignment = NSTextBlockBottomAlignment;
                break;
            };
            default:
                break;
        }
        NSLineBreakMode lineBreakOpts;
        switch (font->desc.wrapping) {
            case FontDescriptor::WrapByWord : {
                lineBreakOpts = NSLineBreakByWordWrapping;
                break;
            }
            case FontDescriptor::WrapByCharacter : {
                lineBreakOpts = NSLineBreakByCharWrapping;
                break;
            };
            case FontDescriptor::None : {
                lineBreakOpts = NSLineBreakByClipping;
                break;
            };
            default:
                break;
        }
        
        [paragraphStyle setAlignment:hAlignment];
        [paragraphStyle setLineBreakMode:lineBreakOpts];
        for(NSTextBlock *textBlock in paragraphStyle.textBlocks){
            [textBlock setVerticalAlignment:vAlignment];
        };
        
        float scaleFactor = [NSScreen mainScreen].backingScaleFactor;
        auto ftextRect = FRect(float(rect.pos.x) * scaleFactor,float(rect.pos.y) * scaleFactor,float(rect.dimen.minWidth) * scaleFactor,float(rect.dimen.minHeight) * scaleFactor);
        CoreTextFont *fontRef = (CoreTextFont *)font.get();
        strData = [[NSAttributedString alloc] initWithString:Native::Cocoa::core_string_to_ns_string(text_val) attributes:@{NSParagraphStyleAttributeName:paragraphStyle,NSFontAttributeName:(__bridge id)fontRef->native}];
        // Draw Text to CGBitmap!
        framesetterRef = CTFramesetterCreateWithAttributedString((__bridge CFAttributedStringRef)strData);
        frame = CTFramesetterCreateFrame(framesetterRef,CFRangeMake(0,0),CGPathCreateWithRect(CGRectMake(ftextRect.pos.x,ftextRect.pos.y, ftextRect.dimen.minWidth, ftextRect.dimen.minHeight),NULL),NULL);
        // CFRetain(frame);
    };
    void * getNative(){
        return (void *)frame;
    };
    void getGlyphBoundingBoxes(Core::Rect **rects, unsigned * count){
        *count = 0;
        CoreTextFont *fontRef = (CoreTextFont *)font.get();
        CFArrayRef lines = CTFrameGetLines(frame);
        for(unsigned idx = 0;idx < CFArrayGetCount(lines);idx++){
            CTLineRef line = (CTLineRef)CFArrayGetValueAtIndex(lines,idx);
            CFArrayRef runs =  CTLineGetGlyphRuns(line);
            for(unsigned j = 0;j < CFArrayGetCount(runs);j++){
                CTRunRef run = (CTRunRef)CFArrayGetValueAtIndex(runs,j);
                const CGGlyph *ptr = CTRunGetGlyphsPtr(run);
                CFIndex count = CTRunGetGlyphCount(run);
                CGRect *rect = new CGRect[count];
                CTFontGetBoundingRectsForGlyphs(fontRef->native,kCTFontOrientationDefault,ptr,rect,count);
            };
        }
    };
    ~CTTextRect(){
        // CFRelease(frame);
        // CFRelease(framesetterRef);
        // [strData release];
    };
};

Core::SharedPtr<TextRect> TextRect::Create(Core::String & _val,Core::SharedPtr<Font> & font,Core::Rect rect){
    return std::make_shared<CTTextRect>(_val,font,rect);
};

Core::SharedPtr<Font> FontEngine::CreateFont(FontDescriptor & desc){
    CTFontRef ref = CTFontCreateWithNameAndOptions(core_string_to_cf_string_ref(desc.family),CGFloat(desc.size),NULL,kCTFontOptionsPreferSystemFont);
    CTFontSymbolicTraits fontTraits;
    
    switch (desc.style) {
        case FontDescriptor::Bold:
            fontTraits = kCTFontTraitBold;
            break;
        case FontDescriptor::Italic:
            fontTraits = kCTFontTraitItalic;
            break;
        case FontDescriptor::BoldAndItalic:
            fontTraits = kCTFontTraitBold | kCTFontTraitItalic;
            break;
        case FontDescriptor::Regular :
            fontTraits = 0;
            break;
        default:
            break;
    }
    
    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    
    CTFontRef _font_final = CTFontCreateCopyWithSymbolicTraits(ref,CGFloat(desc.size) * scaleFactor,NULL,kCTFontTraitBold | kCTFontTraitItalic, fontTraits);
    CFRelease(ref);
    return std::make_shared<CoreTextFont>(desc,_font_final);
};
};
