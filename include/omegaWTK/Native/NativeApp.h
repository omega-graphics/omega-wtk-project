#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEAPP_H
#define OMEGAWTK_NATIVE_NATIVEAPP_H

namespace OmegaWTK::Native {

class NativeAppMenuItem {};

class NativeAppMenu {};

class NativeApp {};

typedef NativeAppMenuItem *NAMI;
typedef NativeAppMenu *NAM;
typedef NativeApp *NativeAppPtr;

NAMI make_menu_item();
NAM make_menu();
    
};

#endif
