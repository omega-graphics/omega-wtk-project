#import <Cocoa/Cocoa.h>
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H
#define OMEGAWTK_NATIVE_MACOS_COCOA_ITEM_H

namespace OmegaWTK::Native {

namespace Cocoa {

class CocoaItem : public NativeItem {
    NSView * _ptr;
public:
    enum : OPT_PARAM {
        View,
        Button
    };
private:
    OPT_PARAM type;
public:
    CocoaItem(NSView *ptr,OPT_PARAM _type);
    ~CocoaItem();
};

};

};

#endif
