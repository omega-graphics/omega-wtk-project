#include "omegaWTK/Composition/Layer.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import "QuartzBackend.h"
#import <QuartzCore/QuartzCore.h>


namespace OmegaWTK::Composition {

#define EIGHTBIT_TO_FLOATING_POINT(val) (val/255.f)
NSColor * color_to_ns_color(Color & color){
    return [NSColor colorWithRed:EIGHTBIT_TO_FLOATING_POINT(color.r) green:EIGHTBIT_TO_FLOATING_POINT(color.g) blue:EIGHTBIT_TO_FLOATING_POINT(color.b) alpha:EIGHTBIT_TO_FLOATING_POINT(color.a)];
};

CFStringRef core_string_to_cf_string(const Core::String & str){
    return CFStringCreateWithCString(CFAllocatorGetDefault(),str.c_str(),kCFStringEncodingUTF8);
};

class QuartzBackend : public Backend {
    
    struct CocoaItemCompAssets {
        Core::Map<unsigned,NSColor *> colors;
        Core::Map<unsigned,CGPathRef> paths;
        Core::Map<unsigned,Core::Rect *> rects;
        Core::Map<unsigned,Core::RoundedRect *> rounded_rects;
        Core::Map<unsigned,Core::Ellipse *> ellipses;
        Core::Map<unsigned,CALayer *> layers;
        Core::Map<unsigned,Text *> text_objects;
        
        template<class _Ty>
        using MapEntry = std::pair<unsigned,_Ty>;
    };
    
    using CocoaItemAssetsCatalog = Core::Map<Native::Cocoa::CocoaItem *,CocoaItemCompAssets>;
    CocoaItemAssetsCatalog assets_catalog;
    
    void setupNSColor(CocoaItemCompAssets *assets,Color & color,unsigned v_id){
        auto found = assets->colors.find(v_id);
        if(found == assets->colors.end()){
            NSColor *ns_color = color_to_ns_color(color);
            assets->colors.insert(CocoaItemCompAssets::MapEntry<NSColor *>(v_id,ns_color));
        }
        else {
//            NSLog(@"Comparing Colors");
            auto & old_color = found->second;
            CGFloat _old_r,_old_g,_old_b,_old_a;
            _old_r = old_color.redComponent;
            _old_g = old_color.greenComponent;
            _old_b = old_color.blueComponent;
            _old_a = old_color.alphaComponent;
            
            CGFloat new_r,new_g,new_b,new_a;
            new_r = EIGHTBIT_TO_FLOATING_POINT(color.r);
            new_g = EIGHTBIT_TO_FLOATING_POINT(color.g);
            new_b = EIGHTBIT_TO_FLOATING_POINT(color.b);
            new_a = EIGHTBIT_TO_FLOATING_POINT(color.a);
            
            if(!((_old_r == new_r) && (_old_g == new_g) && (_old_b == new_b) && (_old_a == new_a))){
                [old_color dealloc];
//                NSLog(@"Setting New Color!");
                old_color = [NSColor colorWithRed:new_r green:new_g blue:new_b alpha:new_a];
            };
        };
    };
    template<class _Layer_Ty>
    bool setupCALayerIfNeeded(CocoaItemCompAssets *assets,unsigned v_id){
        auto found = assets->layers.find(v_id);
        if(found == assets->layers.end()){
            _Layer_Ty * layer = [_Layer_Ty layer];
            assets->layers.insert(CocoaItemCompAssets::MapEntry<CALayer *>(v_id,layer));
            return true;
        };
        return false;
    };
    
    void drawVisual(CocoaItemCompAssets *assets,Visual *visual,unsigned visual_id,CALayer *currentLayer){
        switch (visual->type) {
            case Visual::Rect :
            {
                Visual::RectParams *params = reinterpret_cast<Visual::RectParams *>(visual->params);
                
                bool init = setupCALayerIfNeeded<CAShapeLayer>(assets,visual_id);
                
                CAShapeLayer *shapeLayer = (CAShapeLayer *)assets->layers[visual_id];
                shapeLayer.opaque = YES;
                /// Setup CGPath For Rect! (If needed and if the rect has changed!)
                {
                    auto found = assets->paths.find(visual_id);
                    if(found == assets->paths.end()){
                        assets->rects.insert(CocoaItemCompAssets::MapEntry<Core::Rect *>(visual_id,new Core::Rect(params->rect)));
                        assets->paths.insert(CocoaItemCompAssets::MapEntry<CGPathRef>(visual_id,CGPathCreateWithRect(Native::Cocoa::core_rect_to_cg_rect(params->rect),NULL)));
                    }
                    else {
                        auto & old_path = found->second;
                        auto & old_rect = assets->rects[visual_id];
                        if(!old_rect->compare(params->rect)){
                            CGPathRelease(old_path);
                            old_path = CGPathCreateWithRect(Native::Cocoa::core_rect_to_cg_rect(params->rect),NULL);
                            delete old_rect;
                            old_rect = new Core::Rect(params->rect);
                        };
                        
                    };
                }
                CGPathRef path = assets->paths[visual_id];
                
                setupNSColor(assets,params->color,visual_id);
                NSColor *color = assets->colors[visual_id];
                
                shapeLayer.fillColor = color.CGColor;
                shapeLayer.path = path;
                
                if(init) {
                    [currentLayer addSublayer:shapeLayer];
                    currentLayer = shapeLayer;
                }
                [shapeLayer setNeedsDisplay];
                break;
            }
            case Visual::RoundedRect :
            {
                Visual::RoundedRectParams *params = reinterpret_cast<Visual::RoundedRectParams *>(visual->params);
                bool init = setupCALayerIfNeeded<CAShapeLayer>(assets,visual_id);
                CAShapeLayer *shapeLayer = (CAShapeLayer *)assets->layers[visual_id];
                shapeLayer.opaque = YES;
                /// Setup CGPath For Rounded Rect! (If needed and if the rounded rect has changed!)
                {
                    auto found = assets->paths.find(visual_id);
                    if(found == assets->paths.end()){
                        assets->rounded_rects.insert(CocoaItemCompAssets::MapEntry<Core::RoundedRect *>(visual_id,new Core::RoundedRect(params->rect,params->rad_x,params->rad_y)));
                        assets->paths.insert(CocoaItemCompAssets::MapEntry<CGPathRef>(visual_id,CGPathCreateWithRoundedRect(Native::Cocoa::core_rect_to_cg_rect(params->rect),params->rad_x,params->rad_y,NULL)));
                    }
                    else {
                        auto & old_path = found->second;
                        auto & old_rect = assets->rounded_rects[visual_id];
                        Core::RoundedRect new_rect(params->rect,params->rad_x,params->rad_y);
                        if(!old_rect->compare(new_rect)){
                            CGPathRelease(old_path);
                            old_path = CGPathCreateWithRoundedRect(Native::Cocoa::core_rect_to_cg_rect(params->rect),params->rad_x,params->rad_y,NULL);
                            delete old_rect;
                            old_rect = new Core::RoundedRect(new_rect);
                        };
                    };
                }
                CGPathRef path = assets->paths[visual_id];
                
                setupNSColor(assets,params->color,visual_id);
                NSColor *color = assets->colors[visual_id];
                
                shapeLayer.fillColor = color.CGColor;
                shapeLayer.path = path;
                if(init) {
                    [currentLayer addSublayer:shapeLayer];
                    currentLayer = shapeLayer;
                }
                [shapeLayer setNeedsDisplay];
                break;
            }
            case Visual::Ellipse : {
                Visual::EllipseParams *params = reinterpret_cast<Visual::EllipseParams *>(visual->params);
                bool init = setupCALayerIfNeeded<CAShapeLayer>(assets,visual_id);
                CAShapeLayer *shapeLayer = (CAShapeLayer *)assets->layers[visual_id];
                /// Setup CG Path if needed or if it has changed!
                {
                    auto found = assets->paths.find(visual_id);
                    if(found == assets->paths.end()){
                        assets->ellipses.insert(std::pair<unsigned,Core::Ellipse *>(visual_id,new Core::Ellipse(params->ellipse)));
                        assets->paths.insert(std::pair<unsigned,CGPathRef> (visual_id,CGPathCreateWithEllipseInRect(CGRectMake(params->ellipse.pos.x,params->ellipse.pos.y,params->ellipse.radius_x * 2,params->ellipse.radius_y * 2),NULL)));
                    }
                    else {
                        auto & old_path = found->second;
                        auto & old_ellipse = assets->ellipses[visual_id];
                        if(!old_ellipse->compare(params->ellipse)){
                            CGPathRelease(old_path);
                            old_path = CGPathCreateWithEllipseInRect(CGRectMake(params->ellipse.pos.x,params->ellipse.pos.y, params->ellipse.radius_x * 2,params->ellipse.radius_y * 2),NULL);
                            delete old_ellipse;
                            old_ellipse = new Core::Ellipse(params->ellipse);
                        };
                    };
                    
                    CGPathRef path = assets->paths[visual_id];
                    
                    setupNSColor(assets,params->color,visual_id);
                    NSColor *color = assets->colors[visual_id];
                    
                    shapeLayer.fillColor = color.CGColor;
                    shapeLayer.path = path;
                    
                    if(init) {
                        [currentLayer addSublayer:shapeLayer];
                        currentLayer = shapeLayer;
                    }
                    [shapeLayer setNeedsDisplay];
                    break;
                }
            };
            case Visual::Text : {
                Visual::TextParams *params = reinterpret_cast<Visual::TextParams *>(visual->params);
                bool init = setupCALayerIfNeeded<CATextLayer>(assets,visual_id);
                CATextLayer *textLayer = (CATextLayer *)assets->layers[visual_id];
                /// Setup Text Contiainer if needed or if it has changed!
                {
                    auto found = assets->rects.find(visual_id);
                    if(found == assets->rects.end()){
                        assets->rects.insert(CocoaItemCompAssets::MapEntry<Core::Rect *>(visual_id,new Core::Rect(params->rect)));
                    }
                    else {
                        auto & rect = found->second;
                        if(!rect->compare(params->rect)){
                            delete rect;
                            rect = new Core::Rect(params->rect);
                        };
                    };
                }
                
                auto rect_ptr = assets->rects[visual_id];
                textLayer.frame = Native::Cocoa::core_rect_to_cg_rect(*rect_ptr);
                
                setupNSColor(assets,params->color,visual_id);
                NSColor *color = assets->colors[visual_id];
                textLayer.foregroundColor = color.CGColor;
                /// Setup Text if needed or if needed text has changed!
                bool fontChange;
                {
                    auto found = assets->text_objects.find(visual_id);
                    if(found == assets->text_objects.end()){
                        assets->text_objects.insert(CocoaItemCompAssets::MapEntry<Text *>(visual_id,new Text(params->text)));
                        fontChange = true;
                    }
                    /// Compare Fonts!
                    else {
                        auto & old_font = found->second;
                        auto & new_font = params->text;
                        
                        if(!((old_font->getString() == new_font.getString()) &&
                           (old_font->getFontSize() == new_font.getFontSize()) &&
                           (old_font->getFont().family == new_font.getFont().family) &&
                           (old_font->getFont().style == new_font.getFont().style))){
                            delete old_font;
                            old_font = new Text(new_font);
                            fontChange = true;
                        }
                        else {
                            fontChange = false;
                        };
                    };
                }
                
                if(fontChange) {
                    
                    CFRelease(textLayer.font);
                    /// Redo Font!
                    Text *text = assets->text_objects[visual_id];
                    textLayer.fontSize = text->getFontSize();
                    
                    CTFontSymbolicTraits traits;
                    switch (text->getFont().style) {
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
                            traits = 0;
                        default:
                            break;
                    }
    //
    //
    //                    NSFont *font = [NSFont fontWithDescriptor:fontDescriptor size:params->text.getFontSize()];
                    CTFontRef font = CTFontCreateWithName(core_string_to_cf_string(text->getFont().family),text->getFontSize(), NULL);
                    font = CTFontCreateCopyWithSymbolicTraits(font,text->getFontSize(),NULL,kCTFontTraitBold | kCTFontTraitItalic,traits);
                    textLayer.font = font;
    //                    textLayer.backgroundColor = currentLayer.backgroundColor;
                    textLayer.string = Native::Cocoa::core_string_to_ns_string(text->getString());
                    textLayer.truncationMode = kCATruncationEnd;
                    textLayer.wrapped = YES;
                    textLayer.alignmentMode = kCAAlignmentCenter;
                    textLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
    //                    textLayer.position = Native::Cocoa::core_pos_to_cg_point(params->pos);
                    if(init) {
                        [currentLayer addSublayer:textLayer];
                        currentLayer = textLayer;
                    }
                    [textLayer setNeedsDisplay];
                }
                break;
            }
            default: {
                break;
            }
        };
    };
    
    void doWork(){
        
        Native::Cocoa::CocoaItem * native_ptr = (Native::Cocoa::CocoaItem *)currentLayer->getTargetNativePtr();
        auto & visuals = currentLayer->getTargetVisuals();
        /// Top Level Layer of OmegaWTKCocoaView. DO NOT EDIT!
        CALayer *nativeLayer = native_ptr->getLayer();
        nativeLayer.opaque = YES;
        CALayer *currentLayer = nativeLayer;
        
        CocoaItemCompAssets assets;
        
        auto it = visuals.begin();
        while(it != visuals.end()){
            auto & visual = *it;
            drawVisual(&assets,visual,visual->id,currentLayer);
            ++it;
        };
        assets_catalog.insert(std::pair<Native::Cocoa::CocoaItem *,CocoaItemCompAssets>(native_ptr,assets));
        native_ptr->setNeedsDisplay();
        
    };
    void doUpdate(){
        NSLog(@"Doing Update!");
        Native::Cocoa::CocoaItem * native_ptr = (Native::Cocoa::CocoaItem *)currentLayer->getTargetNativePtr();
        auto & new_visuals = currentLayer->getTargetVisuals();
        /// Top Level Layer of OmegaWTKCocoaView. DO NOT EDIT!
        CALayer *nativeLayer = native_ptr->getLayer();
        nativeLayer.opaque = YES;
        auto assets_found = assets_catalog.find(native_ptr);
        if(assets_found != assets_catalog.end()){
            auto & assets = assets_found->second;
            auto it = new_visuals.begin();
            while(it != new_visuals.end()){
                auto & visual = *it;
                drawVisual(&assets,visual,visual->id,nativeLayer);
                ++it;
            };
            native_ptr->setNeedsDisplay();
        };
    };
};

Backend *make_quartz_backend(){
    return (Backend *)new QuartzBackend();
};

}


