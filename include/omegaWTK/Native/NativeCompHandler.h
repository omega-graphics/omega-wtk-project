#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVECOMPHANDLER_H
#define OMEGAWTK_NATIVE_NATIVECOMPHANDLER_H

namespace OmegaWTK {
    namespace Native {

        class NativeCompositionTask {
            typedef enum : OPT_PARAM {
               Draw,
               Transform,
               Effect
            } TaskType;
            
            NativeCompositionTask(TaskType);
        };
        /// Manages or Passes NativeCompositionTasks to a NativeItem to perform!
        class NativeCompositionHandler {
            Core::Queue<NativeCompositionTask *> taskQueue;
        public:
            void addCompTask(NativeCompositionTask *task);
        };
    }
}

#endif