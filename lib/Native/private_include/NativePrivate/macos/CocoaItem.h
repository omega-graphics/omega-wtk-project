#import <Cocoa/Cocoa.h>
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H
#define OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H

@class OmegaWTKCocoaEventDelegate;

@interface OmegaWTKCocoaView : NSView
-(void)setDelegate:(OmegaWTKCocoaEventDelegate *)delegate;
@end


namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaItem : public NativeItem {
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
    CocoaItem(OmegaWTKCocoaView *ptr,Type _type);
    ~CocoaItem();
};

};

};

#endif
