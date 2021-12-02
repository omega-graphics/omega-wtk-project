#include "omegaWTK/Core/Core.h"

#include "NativeItem.h"

#ifndef OMEGAWTKM_NATIVE_NATIVEVIEWNAVIGATOR_H
#define OMEGAWTKM_NATIVE_NATIVEVIEWNAVIGATOR_H

namespace OmegaWTK::Mobile::Native {

    INTERFACE NativeViewNavigator {
    public:
        INTERFACE_METHOD void addView(NativeItemPtr view) ABSTRACT;
        INTERFACE_METHOD void removeView(NativeItemPtr view) ABSTRACT;
    };
}

#endif