#import <Cocoa/Cocoa.h>
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H
#define OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H

namespace OmegaWTK::Native {
namespace Cocoa {
class CocoaItem;

}
}


@interface OmegaWTKCocoaView : NSView
-(instancetype) initWithFrame:(NSRect) rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *)delegate;
-(CALayer *) getCALayer;
@end

@class CALayer;


namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaItem : public NativeItem {
    Core::Rect rect;
    OmegaWTKCocoaView * _ptr;
    friend class CocoaEventHandler;
public:
    typedef enum : OPT_PARAM {
        View,
        Button
    } Type;
private:
    Type type;
public:
    CALayer *getLayer(){ return [_ptr getCALayer];};
    void setNeedsDisplay();
    CocoaItem(const Core::Rect & rect,Type _type);
    ~CocoaItem();
};

};

};


#endif
