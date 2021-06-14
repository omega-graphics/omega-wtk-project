#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTKM_NATIVE_NATIVEMOBILEAPP_H
#define OMEGAWTKM_NATIVE_NATIVEMOBILEAPP_H

namespace OmegaWTK::Mobile {
    namespace Native {

        class NativeApp {
        public:
            virtual void terminate() = 0;
        };

        typedef NativeApp *NAP;
        NAP make_native_app();

    }
}

#endif