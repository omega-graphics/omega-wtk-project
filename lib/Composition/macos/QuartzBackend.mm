#include "omegaWTK/Composition/Layer.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"

#import "QuartzBackend.h"
#import <QuartzCore/QuartzCore.h>


namespace OmegaWTK::Composition {

NSColor * color_to_ns_color(Color & color){
#define EIGHTBIT_TO_FLOATING_POINT(val) (val/255)
    return [NSColor colorWithRed:EIGHTBIT_TO_FLOATING_POINT(color.r) green:EIGHTBIT_TO_FLOATING_POINT(color.g) blue:EIGHTBIT_TO_FLOATING_POINT(color.b) alpha:EIGHTBIT_TO_FLOATING_POINT(color.a)];
};

class QuartzBackend : public Backend {
    void doWork(){
        
        Native::Cocoa::CocoaItem * native_ptr = (Native::Cocoa::CocoaItem *)currentLayer->getTargetNativePtr();
        auto & visuals = currentLayer->getTargetVisuals();
        CALayer *nativeLayer = native_ptr->getLayer();
        CALayer *currentLayer = nativeLayer;
        auto it = visuals.begin();
        while(it != visuals.end()){
            auto & visual = *it;
            switch (visual->type) {
                case Visual::Rect :
                {
                    Visual::RectParams *params = reinterpret_cast<Visual::RectParams *>(visual->params);
                    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
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
                    textLayer.fontSize = params->size;
                    textLayer.font = [NSFont fontWithName:@"Arial" size:params->size];
                    textLayer.opaque = YES;
                    textLayer.backgroundColor = currentLayer.backgroundColor;
                    textLayer.string = [[NSAttributedString alloc] initWithString:Native::Cocoa::core_string_to_ns_string(params->str)];
                    textLayer.foregroundColor = color_to_ns_color(params->textColor).CGColor;
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


