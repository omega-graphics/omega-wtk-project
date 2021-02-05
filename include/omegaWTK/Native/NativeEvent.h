#include "omegaWTK/Core/Core.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_NATIVEEVENT_H
#define OMEGAWTK_NATIVE_NATIVEEVENT_H

namespace OmegaWTK {
namespace Native {

typedef void* NativeEventParams;

class NativeEvent {
    friend class NativeEventProcessor;
public:
    typedef enum : OPT_PARAM {
        CursorEnter,
        CursorExit,
        LMouseDown,
        LMouseUp,
        RMouseDown,
        RMouseUp,
        KeyDown,
        KeyUp,
        Redraw
    } EventType;
private:
    EventType type;
    NativeEventParams params;
public:
    NativeEvent(EventType _type,NativeEventParams params):type(_type),params(params){};
};

typedef NativeEvent * NativeEventPtr;

typedef struct {
    
} CursorEnterParams;
typedef struct {
    
} CursorExitParams;
typedef struct {
    
} LMouseDownParams;
typedef struct {
    
} LMouseUpParams;
typedef struct {
    
} RMouseDownParams;
typedef struct {
    
} RMouseUpParams;


class NativeEventProcessor;
//
class NativeEventEmitter {
    NativeEventProcessor *message_reciever;
    friend class NativeEventProcessor;
public:
    NativeEventEmitter();
    bool hasReciever();
    void setReciever(NativeEventProcessor *_responder);
    void emit(NativeEventPtr event);
    ~NativeEventEmitter();
};

/// Listens for native events! (Only some types!)
class NativeEventProcessor {
    Core::Queue<NativeEventPtr> message_queue;
    void queue(NativeEventPtr ptr);
    Core::List<NativeEvent::EventType> event_types;
    protected:
    NativeEventPtr getLatest();
    public:
    void onRecieveEvent(NativeEventPtr event);
    NativeEventProcessor(const Core::List<NativeEvent::EventType> & events_to_listen_for = {NativeEvent::CursorEnter,NativeEvent::CursorExit});
    ~NativeEventProcessor();
};
    
};
};

#endif
