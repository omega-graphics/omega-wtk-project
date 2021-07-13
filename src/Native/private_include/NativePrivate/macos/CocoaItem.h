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

@interface OmegaWTKCocoaViewController : NSViewController
-(instancetype) initWithFrame:(NSRect) rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *) delegate;
-(void)setClass:(Class)cls;
@end

@class CALayer;


namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaItem : public NativeItem {
    OmegaWTKCocoaView * _ptr;
    OmegaWTKCocoaViewController *cont;
    NSScrollView *scrollView;
    friend class CocoaEventHandler;
    void enable();
    void disable();
    void addChildNativeItem(NativeItem *nativeItem);
    void removeChildNativeItem(NativeItem *nativeItem);

    void setClippedView(NativeItem *clippedView);
    void toggleHorizontalScrollBar(bool &state);
    void toggleVerticalScrollBar(bool &state);
public:
    Core::Rect rect;
    typedef enum : OPT_PARAM {
        View,
        ScrollView
    } Type;
private:
    Type type;
public:
    bool isReady;
    void resize(Core::Rect &newRect);
    CALayer *getLayer(){ return [_ptr getCALayer];};
    void setNeedsDisplay();
    void * getBinding();
    CocoaItem(const Core::Rect & rect,Type _type,CocoaItem *parent);
    ~CocoaItem();
};

};

};


#endif