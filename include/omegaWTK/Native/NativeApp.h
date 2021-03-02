#include "omegaWTK/Core/Core.h"
#include "NativeItem.h"

#ifndef OMEGAWTK_NATIVE_NATIVEAPP_H
#define OMEGAWTK_NATIVE_NATIVEAPP_H

namespace OmegaWTK::Native {

class NativeApp {
public:
    virtual void terminate() = 0;
    NativeApp();
    ~NativeApp();
};
typedef NativeApp *NAP;

NAP make_native_app();
//void free_native_app();

// NAMI make_menu_item(Core::String & str,bool hasSubMenu,bool isSeperator = false,NAM subMenuPtr = nullptr);
// NAM make_menu();

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void * __create_hwnd_factory(void *hinst);
void * __hwnd_factory_get_all_hwnds(void * hwnd_factory);
void __free_hwnd_factory(void *hwnd_factory);
RECT __get_hwnd_real_coords(HWND hwnd);
#endif
#endif


// #ifdef TARGET_MACOS
// int cocoa_app_init(int argc,char * argv[]);
// #endif};

#endif
