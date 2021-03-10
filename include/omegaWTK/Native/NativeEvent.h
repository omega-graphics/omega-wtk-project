#include "omegaWTK/Core/Core.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_NATIVEEVENT_H
#define OMEGAWTK_NATIVE_NATIVEEVENT_H

namespace OmegaWTK {
namespace Native {

typedef void* NativeEventParams;

class NativeEvent {
public:
    typedef enum : OPT_PARAM {
        /**View Events*/
        HasLoaded,
        CursorEnter,
        CursorExit,
        LMouseDown,
        LMouseUp,
        RMouseDown,
        RMouseUp,
        KeyDown,
        KeyUp,
        /** Window Events*/
        WindowWillClose,
        WindowWillResize,
        WindowHasResized,
        WindowHasFinishedResize
    } EventType;
    EventType type;
    NativeEventParams params;
public:
    NativeEvent(EventType _type,NativeEventParams params):type(_type),params(params){};
};

typedef NativeEvent * NativeEventPtr;

typedef struct {
    
} ViewHasLoaded;

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
    public:
    virtual void onRecieveEvent(NativeEventPtr event) = 0;
    NativeEventProcessor();
    ~NativeEventProcessor();
};
    
};
};

#endif
