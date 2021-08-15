#include "omegaWTK/Native/NativeMenu.h"

#ifndef OMEGAWTK_NATIVE_COCOAMENU_H
#define OMEGAWTK_NATIVE_COCOAMENU_H

namespace OmegaWTK::Native {

namespace Cocoa {

NMI make_cocoa_menu_item(const OmegaCommon::String & str,NM parent,bool hasSubMenu,NM subMenu);
NMI make_cocoa_menu_seperator();
NM make_cocoa_menu(const OmegaCommon::String & name);

};
};

#endif
