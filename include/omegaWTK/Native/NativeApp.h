#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEAPP_H
#define OMEGAWTK_NATIVE_NATIVEAPP_H

namespace OmegaWTK::Native {

class NativeAppMenuItem {};

typedef NativeAppMenuItem *NAMI;

class NativeAppMenu {
    public:
    virtual void addMenuItem(NAMI menu_item,unsigned idx);
};

typedef NativeAppMenu *NAM;

class NativeApp {};

typedef NativeApp *NativeAppPtr;



NAMI make_menu_item(Core::String & str,bool hasSubMenu,NAM subMenuPtr = nullptr);
NAM make_menu();
    
};

#endif
