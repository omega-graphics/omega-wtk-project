#include "omegaWTK/Native/NativeCompHandler.h"

namespace OmegaWTK::Native {

    void NativeCompositionHandler::addCompTask(NativeCompositionTask *task){
        taskQueue.push(task);
    };

}