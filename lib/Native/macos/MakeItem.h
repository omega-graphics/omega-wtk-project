#include "omegaWTK/Native/NativeItem.h"

#import <Cocoa/Cocoa.h>

#ifndef OMEGAWTK_NATIVE_MACOS_MAKEITEM_H
#define OMEGAWTK_NATIVE_MACOS_MAKEITEM_H


namespace OmegaWTK::Native {

namespace Cocoa {
void ns_string_to_core_string(NSString *str,Core::String & res);
NSString * core_string_to_ns_string(Core::String & str);
NSRect core_rect_to_cg_rect(Core::Rect & rect);
NativeItemPtr make_item_cocoa(Core::Rect & rect,OPT_PARAM type);
};

}

#endif
