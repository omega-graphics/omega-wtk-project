#include "omegaWTK/Core/Core.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_NATIVEEVENT_H
#define OMEGAWTK_NATIVE_NATIVEEVENT_H

namespace OmegaWTK {
namespace Native {

class NativeEvent {
    friend class NativeEventResponder;
public:
    typedef enum : OPT_PARAM {
        CursorEnter,
        CursorExit,
        LMouseDown,
        LMouseUp,
        RMouseDown,
        RMouseUp,
        KeyDown,
        KeyUp
    } EventType;
private:
    EventType type;
public:
    NativeEvent(EventType _type):type(_type){};
};

typedef NativeEvent * NativeEventPtr;
// typedef std::function<void(NativeEventPtr)> NativeEventCallback;
// typedef Core::Map<NativeEvent::EventType,NativeEventCallback> NativeEventListenerMap;

///
class NativeEventResponder;
//
class NativeEventEmitter {
    NativeEventResponder *message_reciever;
    friend class NativeEventResponder;
public:
    NativeEventEmitter();
    bool hasReciever();
    void setReciever(NativeEventResponder *_responder);
    void emit(NativeEventPtr event);
    ~NativeEventEmitter();
};

/// Listens for native events! (Only some types!)
class NativeEventResponder {
    Core::Queue<NativeEventPtr> message_queue;
    void queue(NativeEventPtr ptr);
    Core::List<NativeEvent::EventType> event_types;
    protected:
    NativeEventPtr getLatest();
    public:
    void onRecieveEvent(NativeEventPtr event);
    NativeEventResponder(const Core::List<NativeEvent::EventType> & events_to_listen_for = {NativeEvent::CursorEnter,NativeEvent::CursorExit});
    ~NativeEventResponder();
};
    
};
};

#endif
