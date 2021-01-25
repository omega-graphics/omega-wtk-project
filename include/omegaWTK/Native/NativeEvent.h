#include "omegaWTK/Core/Core.h"
#include "NativeItem.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_NATIVEEVENT_H
#define OMEGAWTK_NATIVE_NATIVEEVENT_H

namespace OmegaWTK {
namespace Native {

class NativeEvent {
public:
    typedef enum : OPT_PARAM {
        AnimKeyFrame,
        Hover,
        Click
    } EventType;
private:
    EventType type;
public:
    NativeEvent(EventType _type):type(_type){};
};

typedef NativeEvent *NativeEventPtr;
typedef std::function<void(NativeEventPtr)> NativeEventCallback;
typedef Core::Map<NativeEvent::EventType,NativeEventCallback> NativeEventListenerMap;

class NativeEventHandler {
protected:
    NativeEventListenerMap listeners;
public:
    NativeEventHandler();
    void addListener(NativeEvent::EventType event_type,NativeEventCallback & callback);
    virtual void listen();
    virtual ~NativeEventHandler();
};


typedef NativeEventHandler *NativeEventHandlerPtr;
    
};
};

#endif
