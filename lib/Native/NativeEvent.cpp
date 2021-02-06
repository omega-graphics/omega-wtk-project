#include "omegaWTK/Native/NativeEvent.h"

namespace OmegaWTK::Native {

NativeEventEmitter::NativeEventEmitter():message_reciever(nullptr){

};

NativeEventEmitter::~NativeEventEmitter(){};

bool NativeEventEmitter::hasReciever(){
    return message_reciever != nullptr;
};

void NativeEventEmitter::setReciever(NativeEventProcessor *_responder){
    message_reciever = _responder;
};

void NativeEventEmitter::emit(NativeEventPtr event){
    if(hasReciever())
        message_reciever->onRecieveEvent(event);
};

NativeEventProcessor::NativeEventProcessor(){};


//void NativeEventProcessor::onRecieveEvent(NativeEventPtr event){
//
//};

NativeEventProcessor::~NativeEventProcessor(){
    
};

};
