#include "omegaWTK/Native/NativeEvent.h"

namespace OmegaWTK::Native {

NativeEventEmitter::NativeEventEmitter():message_reciever(nullptr){

};

bool NativeEventEmitter::hasReciever(){
    return message_reciever != nullptr;
};

void NativeEventEmitter::setReciever(NativeEventResponder *_responder){
    message_reciever = _responder;
};

void NativeEventEmitter::emit(NativeEventPtr event){
    message_reciever->onRecieveEvent(event);
};

NativeEventResponder::NativeEventResponder(const Core::List<NativeEvent::EventType> & events_to_listen_for):event_types(events_to_listen_for){};

void NativeEventResponder::queue(NativeEventPtr event){
    message_queue.push(event);
};

NativeEventPtr NativeEventResponder::getLatest(){
    auto eventPtr = message_queue.front();
    message_queue.pop();
    return eventPtr;
};

void NativeEventResponder::onRecieveEvent(NativeEventPtr event){
    for(auto & t : event_types){
        if(t == event->type){
            queue(event);
        };
    };
};

};
