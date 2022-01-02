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

@interface OmegaWTKCocoaScrollViewDelegate : NSObject
@end

@class CALayer;


namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaItem : public NativeItem {
    OmegaWTKCocoaView * _ptr;
    OmegaWTKCocoaViewController *cont;
    NSScrollView *scrollView;
    OmegaWTKCocoaScrollViewDelegate *scrollViewDelegate;
    friend class CocoaEventHandler;
    void enable() override;
    void disable() override;
    void addChildNativeItem(NativeItemPtr nativeItem) override;
    void removeChildNativeItem(NativeItemPtr nativeItem) override;

    void setClippedView(NativeItemPtr clippedView) override;
    void toggleHorizontalScrollBar(bool &state) override;
    void toggleVerticalScrollBar(bool &state) override;
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
    void resize(const Core::Rect &newRect) override;
    CALayer *getLayer(){ return [_ptr getCALayer];};
    void setNeedsDisplay();
    void * getBinding() override;
    Core::Rect & getRect() override {
        return rect;
    }
    explicit CocoaItem(const Core::Rect & rect,Type _type,SharedHandle<CocoaItem> parent);
    ~CocoaItem();
};

};

};


#endif
