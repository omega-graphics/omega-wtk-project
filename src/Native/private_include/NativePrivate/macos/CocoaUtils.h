#include "omegaWTK/Core/Core.h"

#import <Cocoa/Cocoa.h>

#ifndef OMEGAWTK_NATIVE_MACOS_COCOAUTILS_H
#define OMEGAWTK_NATIVE_MACOS_COCOAUTILS_H

namespace OmegaWTK::Native {
namespace Cocoa {

void ns_string_to_common_string(NSString *str,OmegaCommon::String & res);
NSString * common_string_to_ns_string(const OmegaCommon::String & str);
NSRect core_rect_to_cg_rect(const Core::Rect & rect);
CGPoint core_pos_to_cg_point(const Core::Position & pos);

};
};

#endif
