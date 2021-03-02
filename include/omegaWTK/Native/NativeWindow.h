#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEWINDOW_H
#define OMEGAWTK_NATIVE_NATIVEWINDOW_H

namespace OmegaWTK::Native {
    class NativeWindow {
        #ifdef TARGET_MACOS
        protected:
        Core::Rect & rect;
        public:
        NativeWindow(Core::Rect & rect);
        #endif
    };
    typedef NativeWindow *NWH;
    NWH make_native_window(Core::Rect & rect);
};

#endif