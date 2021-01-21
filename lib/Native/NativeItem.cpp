#include "omegaWTK/Native/NativeItem.h"

#ifdef TARGET_WIN32
#include "win/MakeItem.h" 
#endif

#ifdef TARGET_MACOS
#include "macos/MakeItem.h"
#endif

namespace OmegaWTK::Native {
    NativeItemPtr make_native_item(Core::Dimensions dimen,OPT_PARAM type){
        #ifdef TARGET_WIN32
        return Win::make_item_win(dimen,type); 
        #endif
        #ifdef TARGET_MACOS
        return Mac::make_item_mac(dimen,type);
        #endif
    };
};