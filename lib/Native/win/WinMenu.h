#include "omegaWTK/Native/NativeMenu.h"


#ifndef OMEGAWTK_NATIVE_WIN_WINMENU_H
#define OMEGAWTK_NATIVE_WIN_WINMENU_H

namespace OmegaWTK::Native {
    namespace Win {
        NM make_win_menu(const Core::String &name);
        NMI make_win_menu_item(const Core::String & str,NM parent,bool hasSubMenu,NM subMenu);
        NMI make_win_menu_seperator();

        void select_item(void * menu,unsigned idx);
    };
};


#endif