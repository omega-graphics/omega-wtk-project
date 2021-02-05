#include "omegaWTK/Native/NativeApp.h"

#ifdef TARGET_WIN32
#include "win/WinMenu.h"
#endif

#ifdef TARGET_MACOS
#include "macos/CocoaApp.h"
#endif

namespace OmegaWTK::Native {

// NAMI make_menu_item(Core::String & str,bool hasSubMenu,bool isSeperator,NAM subMenuPtr){
// #ifdef TARGET_WIN32
//     return NAMI(new Win::WinMenuItem(str,hasSubMenu,isSeperator,subMenuPtr));
// #endif

// return NAMI(nullptr);
// //#ifdef TARGET_MACOS
// //    return NAMI(new Cocoa::CocoaMenuItem());
// //#endif
// };

// NAM make_menu(){
// #ifdef TARGET_WIN32
// return NAM(new Win::WinMenu());
// #endif
    
// return NAM(nullptr);
    
// //#ifdef TARGET_MACOS
// //    return NAM(new Cocoa::CocoaMenu());
// //#endif
// };

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

// #ifdef TARGET_MACOS
// int cocoa_app_init(int argc,char * argv[]){
//     return Native::Cocoa::nsapp_init(argc,const_cast<const char **>(argv));
// };
// #endif

};

#ifdef TARGET_WIN32
#ifdef WINDOWS_PRIVATE

#include "win/HWNDFactory.h"

void *__create_hwnd_factory(void *hinst,void *hwndroot){
    auto ptr = new OmegaWTK::Native::Win::HWNDFactory((HINSTANCE)hinst,(HWND)hwndroot);
    OmegaWTK::Native::Win::HWNDFactory::appFactoryInst = ptr;
    return ptr;
};

void __free_hwnd_factory(void * hwnd_factory){
    delete (OmegaWTK::Native::Win::HWNDFactory *) hwnd_factory;
};

#endif
#endif
;
