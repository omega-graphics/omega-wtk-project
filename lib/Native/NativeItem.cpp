#include "omegaWTK/Native/NativeItem.h"

#ifdef TARGET_WIN32
#include "win/MakeItem.h" 
#endif

#ifdef TARGET_MACOS
#include "macos/MakeItem.h"
#endif

namespace OmegaWTK::Native {
    NativeItemPtr make_native_item(Core::Rect rect,OPT_PARAM type){
        #ifdef TARGET_WIN32
        return Win::make_item_win(rect,type); 
        #endif
        #ifdef TARGET_MACOS
        return Cocoa::make_item_cocoa(rect,type);
        #endif
    };
};
