#include "omegaWTK/Native/NativeApp.h"

#ifdef TARGET_WIN32
#include "win/WinMenu.h"
#endif

#ifdef TARGET_MACOS
#include "macos/CocoaApp.h"
#endif

namespace OmegaWTK::Native {

NAMI make_menu_item(Core::String & str,bool hasSubMenu,bool isSeperator,NAM subMenuPtr){
#ifdef TARGET_WIN32
    return NAMI(new Win::WinMenuItem(str,hasSubMenu,isSeperator,subMenuPtr));
#endif

return NAMI(nullptr);
//#ifdef TARGET_MACOS
//    return NAMI(new Cocoa::CocoaMenuItem());
//#endif
};

NAM make_menu(){
#ifdef TARGET_WIN32
return NAM(new Win::WinMenu());
#endif
    
return NAM(nullptr);
    
//#ifdef TARGET_MACOS
//    return NAM(new Cocoa::CocoaMenu());
//#endif
};

NativeApp::NativeApp(){
    
};

void NativeApp::setNativeItemPtr(NativeItemPtr _ptr){
    ptr = _ptr;
};

void * NativeApp::getNativeItemNativeBinding(){
    return ptr->getBinding();
};

NativeApp::~NativeApp(){
    
};

int cocoa_app_init(int argc,char * argv[]){
    return Native::Cocoa::nsapp_init(argc,const_cast<const char **>(argv));
};

};
