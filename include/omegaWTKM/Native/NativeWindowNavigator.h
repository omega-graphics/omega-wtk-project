#include "omegaWTK/Core/Core.h"

#include "NativeItem.h"

#ifndef OMEGAWTKM_NATIVE_NATIVEVIEWNAVIGATOR_H
#define OMEGAWTKM_NATIVE_NATIVEVIEWNAVIGATOR_H

namespace OmegaWTK::Mobile::Native {

    INTERFACE NativeWindow {
    public:
        INTERFACE_METHOD void setRootView(NativeItemPtr view) ABSTRACT;

        static SharedHandle<NativeWindow> Create();
    };

    typedef SharedHandle<NativeWindow> NativeWindowPtr;

    INTERFACE NativeWindowNavigator {
    public:
        INTERFACE_METHOD NativeWindowPtr newWindow() ABSTRACT;

        INTERFACE_METHOD void setKeyWindow(unsigned idx) ABSTRACT;
    };
}

#endif