#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVETHEME_H
#define OMEGAWTK_NATIVE_NATIVETHEME_H

namespace OmegaWTK::Native {

    struct ThemeDesc {

    };

    class NativeThemeObserver {
    public:
        INTERFACE_METHOD void onThemeSet(ThemeDesc & desc) ABSTRACT;
        virtual ~NativeThemeObserver() = default;
    };



    OMEGAWTK_EXPORT ThemeDesc queryCurrentTheme();

    
};

#endif