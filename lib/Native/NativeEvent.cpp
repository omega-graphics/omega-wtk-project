#include "omegaWTK/Native/NativeEvent.h"

namespace OmegaWTK::Native {

NativeEventHandler::NativeEventHandler(){};

void NativeEventHandler::addListener(NativeEvent::EventType event_type, NativeEventCallback & callback){
    listeners.insert(std::pair<NativeEvent::EventType,NativeEventCallback> (event_type,callback));
};

};
