#include "omegaWTK/Native/NativeEvent.h"

namespace OmegaWTK::Native {

NativeEventHandler::NativeEventHandler(){};

void NativeEventHandler::addListener(OPT_PARAM event_type, NativeEventCallback & callback){
    listeners.insert(std::pair<OPT_PARAM,NativeEventCallback> (event_type,callback));
};

};
