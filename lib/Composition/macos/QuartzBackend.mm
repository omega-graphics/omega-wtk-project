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
        auto tasks = currentLayer->getTargetTasks();
        CALayer *nativeLayer = native_ptr->getLayer();
        CALayer *currentLayer = nativeLayer;
        while(!tasks.empty()){
            auto task = tasks.front();
            tasks.pop();
            switch (task->type) {
                case Task::DrawRect :
                {
                    Task::DrawRectParams *params = reinterpret_cast<Task::DrawRectParams *>(task->params);
                    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
                    CGMutablePathRef path = CGPathCreateMutable();
                    CGPathAddRect(path,&CGAffineTransformIdentity,Native::Cocoa::core_rect_to_cg_rect(params->rect));
                    shapeLayer.fillColor = color_to_ns_color(params->color).CGColor;
                    shapeLayer.path = path;
                    [currentLayer addSublayer:shapeLayer];
                    currentLayer = shapeLayer;
                    break;
                }
                case Task::DrawRoundedRect :
                {
                    Task::DrawRoundedRectParams *params = reinterpret_cast<Task::DrawRoundedRectParams *>(task->params);
                    CAShapeLayer *shapeLayer = [CAShapeLayer layer];
                    CGMutablePathRef path = CGPathCreateMutable();
                    CGPathAddRoundedRect(path,&CGAffineTransformIdentity,Native::Cocoa::core_rect_to_cg_rect(params->rect), params->rad_x,params->rad_y);
                    shapeLayer.fillColor = color_to_ns_color(params->color).CGColor;
                    shapeLayer.path = path;
                    [currentLayer addSublayer:shapeLayer];
                    currentLayer = shapeLayer;
                    break;
                }
                case Task::DrawText : {
                    Task::DrawTextParams *params = reinterpret_cast<Task::DrawTextParams *>(task->params);
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
                default:
                    break;
            }
        };
        native_ptr->setNeedsDisplay();
    };
};

Backend *make_quartz_backend(){
    return (Backend *)new QuartzBackend();
};

}


