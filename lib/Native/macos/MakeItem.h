#include "omegaWTK/Native/NativeItem.h"

#import <Cocoa/Cocoa.h>

#ifndef OMEGAWTK_NATIVE_MACOS_MAKEITEM_H
#define OMEGAWTK_NATIVE_MACOS_MAKEITEM_H


namespace OmegaWTK::Native {

namespace Cocoa {

NativeItemPtr make_item_cocoa(Core::Rect & rect,OPT_PARAM type);
};

}

#endif
