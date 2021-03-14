#include "omegaWTK/Native/NativeMenu.h"

#ifndef OMEGAWTK_NATIVE_GTK_GTKMENU_H
#define OMEGAWTK_NATIVE_GTK_GTKMENU_H

namespace OmegaWTK::Native::GTK {

NMI make_gtk_menu_item(const Core::String & str,NM parent,bool hasSubMenu,NM subMenu);
NMI make_gtk_menu_seperator();
NM make_gtk_menu(const Core::String & name);
};

#endif