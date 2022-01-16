#include "omegaWTK/Core/Core.h"
#include "NativeWindowNavigator.h"

#ifndef OMEGAWTKM_NATIVE_NATIVEMOBILEAPP_H
#define OMEGAWTKM_NATIVE_NATIVEMOBILEAPP_H

namespace OmegaWTK::Mobile {
    namespace Native {

        INTERFACE NativeApp {
        public:
            INTERFACE_METHOD SharedHandle<NativeWindowNavigator> createNavigator() ABSTRACT;
            INTERFACE_METHOD void setDefaultNavigator(SharedHandle<NativeWindowNavigator> & navigator) ABSTRACT;
            INTERFACE_METHOD int run() ABSTRACT;
            INTERFACE_METHOD void terminate() ABSTRACT;
        };

        typedef SharedHandle<NativeApp> NAP;
        NAP make_native_app();

    }
}

#endif