#include "omegaWTK/Native/NativeApp.h"

#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_WINMENU_H
#define OMEGAWTK_NATIVE_WIN_WINMENU_H

namespace OmegaWTK::Native {
    namespace Win {
        class WinMenuItem : public NativeAppMenuItem {
            MENUITEMINFO menu_item_info;
            public:
            WinMenuItem();
            ~WinMenuItem();
        };
        class WinMenu : public NativeAppMenu {
            HMENU hMenu;
            public:
            WinMenu();
            ~WinMenu();
        };
    };
};


#endif