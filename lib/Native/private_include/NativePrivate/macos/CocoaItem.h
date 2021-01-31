#import <Cocoa/Cocoa.h>
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H
#define OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H



@class OmegaWTKCocoaView;


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
    CocoaItem(Core::Rect & rect,Type _type);
    ~CocoaItem();
};

};

};

@interface OmegaWTKCocoaView : NSView
-(instancetype) initWithFrame:(NSRect) rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *)delegate;
@end

#endif
