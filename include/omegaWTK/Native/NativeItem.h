#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEITEM_H
#define OMEGAWTK_NATIVE_NATIVEITEM_H

namespace OmegaWTK {
    namespace Native {
        /// Null Object Placeholder for native object!
        class NativeItem {};
        typedef NativeItem *NativeItemPtr;
        
        typedef enum : OPT_PARAM {
            Default,
            PushButton,
            TextInput 
        }ItemType;

        NativeItemPtr make_native_item(Core::Rect rect,OPT_PARAM type = Native::Default);
    }
};

#endif
