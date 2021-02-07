#include "omegaWTK/Composition/Layer.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import "QuartzBackend.h"
#import <QuartzCore/QuartzCore.h>


namespace OmegaWTK::Composition {

NSColor * color_to_ns_color(Color & color){
#define EIGHTBIT_TO_FLOATING_POINT(val) (val/255.f)
    return [NSColor colorWithRed:EIGHTBIT_TO_FLOATING_POINT(color.r) green:EIGHTBIT_TO_FLOATING_POINT(color.g) blue:EIGHTBIT_TO_FLOATING_POINT(color.b) alpha:EIGHTBIT_TO_FLOATING_POINT(color.a)];
};

CFStringRef core_string_to_cf_string(const Core::String & str){
    return CFStringCreateWithCString(CFAllocatorGetDefault(),str.c_str(),kCFStringEncodingUTF8);
};

class QuartzBackend : public Backend {
    void doWork(){
        
        Native::Cocoa::CocoaItem * native_ptr = (Native::Cocoa::CocoaItem *)currentLayer->getTargetNativePtr();
        auto & visuals = currentLayer->getTargetVisuals();
        CALayer *nativeLayer = native_ptr->getLayer();
        nativeLayer.opaque = YES;
        CALayer *currentLayer = nativeLayer;
        auto it = visuals.begin();
        while(it != visuals.end()){
            auto & visual = *it;
            switch (visual->type) {
                case Visual::Rect :
                {
                    Visual::RectParams *params = reinterpret_cast<Visual::RectParams *>(visual->params);
                    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
                    shapeLayer.opaque = YES;
                    CGMutablePathRef path = CGPathCreateMutable();
                    CGPathAddRect(path,&CGAffineTransformIdentity,Native::Cocoa::core_rect_to_cg_rect(params->rect));
                    shapeLayer.fillColor = color_to_ns_color(params->color).CGColor;
                    shapeLayer.path = path;
                    [currentLayer addSublayer:shapeLayer];
                    currentLayer = shapeLayer;
                    break;
                }
                case Visual::RoundedRect :
                {
                    Visual::RoundedRectParams *params = reinterpret_cast<Visual::RoundedRectParams *>(visual->params);
                    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
                    shapeLayer.opaque = YES;
                    CGMutablePathRef path = CGPathCreateMutable();
                    CGPathAddRoundedRect(path,&CGAffineTransformIdentity,Native::Cocoa::core_rect_to_cg_rect(params->rect), params->rad_x,params->rad_y);
                    shapeLayer.fillColor = color_to_ns_color(params->color).CGColor;
                    shapeLayer.path = path;
                    [currentLayer addSublayer:shapeLayer];
                    currentLayer = shapeLayer;
                    break;
                }
                case Visual::Text : {
                    Visual::TextParams *params = reinterpret_cast<Visual::TextParams *>(visual->params);
                    CATextLayer *textLayer = [CATextLayer layer];
                    textLayer.frame = Native::Cocoa::core_rect_to_cg_rect(params->rect);
                    textLayer.fontSize = params->text.getFontSize();
                    
                    CTFontSymbolicTraits traits;
                    switch (params->text.getFont().style) {
                        case Text::Font::Bold : {
                            traits = kCTFontTraitBold;
                            break;
                        };
                        case Text::Font::BoldAndItalic : {
                            traits = kCTFontTraitBold | kCTFontTraitItalic;
                            break;
                        };
                        case Text::Font::Italic : {
                            traits = kCTFontTraitItalic;
                            break;
                        };
                        case Text::Font::Regular:
                        default:
                            break;
                    }
                    
//                    NSFontDescriptor *fontDescriptor = [[[NSFontDescriptor fontDescriptorWithName:(Native::Cocoa::core_string_to_ns_string(params->text.getFont().family)) size:params->text.getFontSize()] initWithFontAttributes:@{NSFontFamilyAttribute:Native::Cocoa::core_string_to_ns_string(params->text.getFont().family)}] fontDescriptorWithSymbolicTraits:traits];
//
//
//                    NSFont *font = [NSFont fontWithDescriptor:fontDescriptor size:params->text.getFontSize()];
                    CTFontRef font = CTFontCreateWithName(core_string_to_cf_string(params->text.getFont().family),params->text.getFontSize(), NULL);
                    font = CTFontCreateCopyWithSymbolicTraits(font,params->text.getFontSize(),NULL,kCTFontTraitBold | kCTFontTraitItalic,traits);
                    textLayer.font = font;
//                    textLayer.backgroundColor = currentLayer.backgroundColor;
                    textLayer.string = Native::Cocoa::core_string_to_ns_string(params->text.getString());
                    textLayer.truncationMode = kCATruncationEnd;
                    textLayer.foregroundColor = color_to_ns_color(params->color).CGColor;
                    textLayer.wrapped = YES;
                    textLayer.alignmentMode = kCAAlignmentCenter;
                    textLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
//                    textLayer.position = Native::Cocoa::core_pos_to_cg_point(params->pos);
                    [currentLayer addSublayer:textLayer];
                    currentLayer = textLayer;
                    break;
                }
                default: {
                    break;
                }
            };
            ++it;
            native_ptr->setNeedsDisplay();
        };
        
    };
    void doUpdate(){
        doWork();
    };
};

Backend *make_quartz_backend(){
    return (Backend *)new QuartzBackend();
};

}


