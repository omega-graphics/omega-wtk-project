#include "omegaWTK/Native/NativeMenu.h"

#ifdef TARGET_MACOS
#include "macos/CocoaMenu.h"
#endif

#ifdef TARGET_WIN32
#include "win/WinMenu.h"
#endif

namespace OmegaWTK::Native {

void NativeMenu::setDelegate(NativeMenuDelegate *delegate){
    this->delegate = delegate;
    this->hasDelegate = true;
};

NMI make_native_menu_item(const Core::String & str,NM parent,bool hasSubMenu,NM subMenu){
    
#ifdef TARGET_MACOS
    return Cocoa::make_cocoa_menu_item(str,parent,hasSubMenu,subMenu);
#endif
    
};

NMI make_native_menu_seperator(){
#ifdef TARGET_MACOS
    return Cocoa::make_cocoa_menu_seperator();
#endif
};

NM make_native_menu(const Core::String & name){
#ifdef TARGET_MACOS
    return Cocoa::make_cocoa_menu(name);
#endif
};

};
