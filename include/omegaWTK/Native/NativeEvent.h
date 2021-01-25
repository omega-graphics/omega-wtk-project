#include "omegaWTK/Core/Core.h"
#include "NativeItem.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_NATIVEEVENT_H
#define OMEGAWTK_NATIVE_NATIVEEVENT_H

namespace OmegaWTK {
namespace Native {

class NativeEvent {
    OPT_PARAM type;
public:
    enum : OPT_PARAM {
        AnimKeyFrame,
        Hover,
        Click
    };
    NativeEvent(OPT_PARAM _type):type(_type){};
};

typedef NativeEvent *NativeEventPtr;
typedef std::function<void(NativeEventPtr)> NativeEventCallback;
typedef Core::Map<OPT_PARAM,NativeEventCallback> NativeEventListenerMap;

class NativeEventHandler {
protected:
    NativeEventListenerMap listeners;
public:
    NativeEventHandler();
    void addListener(OPT_PARAM event_type,NativeEventCallback & callback);
    virtual void listen();
    virtual ~NativeEventHandler();
};


typedef NativeEventHandler *NativeEventHandlerPtr;
    
};
};

#endif
