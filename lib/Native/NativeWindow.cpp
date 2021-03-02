#include "omegaWTK/Native/NativeWindow.h"

#ifdef TARGET_WIN32
#include "win/MakeItem.h"
#endif

#ifdef TARGET_MACOS
#include "macos/MakeItem.h"
#endif

namespace OmegaWTK::Native {
    #ifdef TARGET_MACOS
    NativeWindow::NativeWindow(Core::Rect & rect):rect(rect){};
    #endif
    NWH make_native_window(Core::Rect & rect){
        #ifdef TARGET_WIN32
            return Win::make_win_window(rect);
        #endif

        #ifdef TARGET_MACOS
        #endif
    };
};