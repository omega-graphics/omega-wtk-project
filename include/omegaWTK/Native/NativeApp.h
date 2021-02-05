#include "omegaWTK/Core/Core.h"
#include "NativeItem.h"

#ifndef OMEGAWTK_NATIVE_NATIVEAPP_H
#define OMEGAWTK_NATIVE_NATIVEAPP_H

namespace OmegaWTK::Native {

// class NativeAppMenuItem {};

// typedef NativeAppMenuItem *NAMI;

// class NativeAppMenu {
//     public:
//     virtual void addMenuItem(NAMI menu_item);
//     virtual void insertMenuItem(NAMI menu_item,unsigned idx);
// };

// typedef NativeAppMenu *NAM;

class NativeApp {
    NativeItemPtr ptr;
public:
    NativeApp();
    void setNativeItemPtr(NativeItemPtr _ptr);
    void *getNativeItemNativeBinding();
    ~NativeApp();
};
typedef NativeApp *NAP;


// NAMI make_menu_item(Core::String & str,bool hasSubMenu,bool isSeperator = false,NAM subMenuPtr = nullptr);
// NAM make_menu();

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE
void * __create_hwnd_factory(void *hinst,void *hwndroot);

void __free_hwnd_factory(void *hwnd_factory);
#endif
#endif


// #ifdef TARGET_MACOS
// int cocoa_app_init(int argc,char * argv[]);
// #endif};

#endif
