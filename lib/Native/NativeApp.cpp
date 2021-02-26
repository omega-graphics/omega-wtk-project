#include "omegaWTK/Native/NativeApp.h"


#ifdef TARGET_MACOS
#include "macos/CocoaApp.h"
#endif

namespace OmegaWTK::Native {

NativeApp::NativeApp(){
    
};

void NativeApp::addNativeItemPtr(NativeItemPtr _ptr){
    root_native_items.push_back(_ptr->getBinding());
};

std::vector<void *> & NativeApp::getNativeItemsWithBindings(){
    return root_native_items;
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
#include "NativePrivate/win/HWNDItem.h"

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

RECT __get_hwnd_real_coords(HWND hwnd){
    auto item = (OmegaWTK::Native::Win::HWNDItem *)OmegaWTK::Native::Win::getHWNDUserData(hwnd);
    RECT rc;
    auto & _rect = item->wndrect;
    rc.left = _rect.pos.x;
    rc.right = _rect.pos.x + _rect.dimen.minWidth;
    rc.bottom = _rect.pos.y;
    rc.top = _rect.pos.y + _rect.dimen.minHeight;
    return rc;
};

#endif
#endif
;
