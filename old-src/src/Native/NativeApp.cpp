#include "omegaWTK/Native/NativeApp.h"

#ifdef TARGET_WIN32
#include "win/WinApp.h"
#endif


#ifdef TARGET_MACOS
#include "macos/CocoaApp.h"
#endif

#ifdef TARGET_GTK
#include "gtk/GTKApp.h"
#endif


namespace OmegaWTK::Native {

NativeApp::NativeApp(){
    
};

NativeApp::~NativeApp(){
    
};

NAP make_native_app(void *data){
#ifdef TARGET_WIN32
    return Win::make_win_app(data);
#endif
    
#ifdef TARGET_MACOS
    return Cocoa::make_cocoa_app();
#endif

#ifdef TARGET_GTK
    return GTK::make_gtk_app();
#endif
};


};

#ifdef TARGET_WIN32
#ifdef WINDOWS_PRIVATE

#include "win/HWNDFactory.h"
#include "NativePrivate/win/HWNDItem.h"

RECT __get_hwnd_real_coords(HWND hwnd){
    auto item = (OmegaWTK::Native::Win::HWNDItem *)OmegaWTK::Native::Win::getHWNDUserData(hwnd);
    RECT rc;
    auto & _rect = item->wndrect;
    rc.left = _rect.pos.x;
    rc.right = _rect.pos.x + _rect.w;
    rc.bottom = _rect.pos.y;
    rc.top = _rect.pos.y + _rect.h;
    return rc;
};

#endif
#endif
;
