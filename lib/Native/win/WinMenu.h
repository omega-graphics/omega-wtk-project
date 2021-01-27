#include "omegaWTK/Native/NativeApp.h"

#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_WINMENU_H
#define OMEGAWTK_NATIVE_WIN_WINMENU_H

namespace OmegaWTK::Native {
    namespace Win {
        class WinMenuItem : public NativeAppMenuItem {
            MENUITEMINFO menu_item_info;
            friend class WinMenu;
            public:
            WinMenuItem(Core::String & name,bool hasSubMenu,bool isSeperator,NAM subMenu);
            ~WinMenuItem();
        };
        class WinMenu : public NativeAppMenu {
            HMENU hMenu;
            MENUINFO menu_info;
            friend class WinMenuItem;
            public:
            void addMenuItem(NAMI menu_item,unsigned idx);
            HMENU getHandle(){return hMenu;};
            WinMenu();
            ~WinMenu();
        };


    };
};


#endif