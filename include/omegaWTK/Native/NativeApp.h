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
    Core::Vector<void *> root_native_items;
public:
    NativeApp();
    void addNativeItemPtr(NativeItemPtr _ptr);
    std::vector<void *> & getNativeItemsWithBindings();
    ~NativeApp();
};
typedef NativeApp *NAP;


// NAMI make_menu_item(Core::String & str,bool hasSubMenu,bool isSeperator = false,NAM subMenuPtr = nullptr);
// NAM make_menu();

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void * __create_hwnd_factory(void *hinst,void *hwndroot);
void * __hwnd_factory_get_all_hwnds(void * hwnd_factory);
void __free_hwnd_factory(void *hwnd_factory);
RECT __get_hwnd_real_coords(HWND hwnd);
#endif
#endif


// #ifdef TARGET_MACOS
// int cocoa_app_init(int argc,char * argv[]);
// #endif};

#endif
