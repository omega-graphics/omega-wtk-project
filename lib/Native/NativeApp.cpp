#include "omegaWTK/Native/NativeApp.h"


#ifdef TARGET_MACOS
#include "macos/CocoaApp.h"
#endif

namespace OmegaWTK::Native {

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

void * __hwnd_factory_get_all_hwnds(void * hwnd_factory){
    return ((OmegaWTK::Native::Win::HWNDFactory *)hwnd_factory)->getAllHwnds();
};

void __free_hwnd_factory(void * hwnd_factory){
    delete (OmegaWTK::Native::Win::HWNDFactory *) hwnd_factory;
};

#endif
#endif
;
