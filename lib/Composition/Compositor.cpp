#include "omegaWTK/Composition/Compositor.h"

#ifdef TARGET_WIN32
#include "win/Compositor_Win32.h"
#endif

#ifdef TARGET_MACOS
#include "macos/Compositor_macOS.h"
#endif

namespace OmegaWTK::Composition {
    
    Compositor * make_compositor(){
        #ifdef TARGET_WIN32
        return new WinCompositor();
        #endif

        #ifdef TARGET_MACOS 
        return new CocoaCompositor();
        #endif
    };
};