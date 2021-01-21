#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEITEM_H
#define OMEGAWTK_NATIVE_NATIVEITEM_H

namespace OmegaWTK {
    namespace Native {
        /// Null Object Placeholder for native object!
        class NativeItem {};
        typedef NativeItem *NativeItemPtr;

        STATIC_OPT Default = 0;
        STATIC_OPT PushButton = 1;
        STATIC_OPT TextInput = 2;

        NativeItemPtr make_native_item(Core::Dimensions dimen,OPT_PARAM type = Native::Default);
    }
};

#endif
