#include "omegaWTK/Native/NativeTheme.h"

#import <AppKit/AppKit.h>

namespace OmegaWTK::Native {
    ThemeDesc queryDefaultTheme(){
        ThemeDesc desc {};
        NSAppearance *defAppearance = NSApp.appearance;
    }
}