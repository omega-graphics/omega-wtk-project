#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVECOMPHANDLER_H
#define OMEGAWTK_NATIVE_NATIVECOMPHANDLER_H

namespace OmegaWTK {
    namespace Native {
    class NativeItem;
        /// Manages or Passes NativeCompositionTasks to a NativeItem to perform!
        class NativeCompositionHandler {
            NativeItem * native_binding;
        public:
            NativeCompositionHandler(NativeItem * native);
            
        };
    }
}

#endif
