#include "omegaWTK/Native/NativeApp.h"

#ifdef TARGET_WIN32
#include "win/WinMenu.h"
#endif

#ifdef TARGET_MACOS
#include "macos/CocoaMenu.h"
#endif

namespace OmegaWTK::Native {

NAMI make_menu_item(Core::String & str,bool hasSubMenu,bool isSeperator,NAM subMenuPtr){
#ifdef TARGET_WIN32
    return NAMI(new Win::WinMenuItem(str,hasSubMenu,isSeperator,subMenuPtr));
#endif
    
#ifdef TARGET_MACOS
    return NAMI(new Cocoa::CocoaMenuItem());
#endif
};

NAM make_menu(){
#ifdef TARGET_WIN32
return NAM(new Win::WinMenu());
#endif
    
#ifdef TARGET_MACOS
    return NAM(new Cocoa::CocoaMenu());
#endif
};

};
