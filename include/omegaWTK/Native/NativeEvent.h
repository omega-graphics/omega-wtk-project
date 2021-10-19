#include "omegaWTK/Core/Core.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_NATIVEEVENT_H
#define OMEGAWTK_NATIVE_NATIVEEVENT_H

namespace OmegaWTK {
namespace Native {

typedef void* NativeEventParams;

class OMEGAWTK_EXPORT NativeEvent {
public:
    typedef enum : OPT_PARAM {
        Unknown,
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

typedef SharedHandle<NativeEvent> NativeEventPtr;

struct WindowWillResize {
    Core::Rect rect;
    WindowWillResize(Core::Rect rect);
};

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

// enum KeyCode : int {
//     // Letter Codes
//     a,b,c,
//     d,e,f,
//     g,h,i,
//     j,k,l,
//     m,n,o,
//     p,q,r,
//     s,t,u,
//     v,w,x
//     ,y,z,
//     // Other Keycodes
//     n1,n2,n3,n4,n5,n6,n7,n8,n9,n0,
//     f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,
//     lshift,rshift,
//     #ifdef TARGET_MACOS
//         lcmd,rcmd,
//     #endif
//     lalt,ralt,
//     tab,
//     exclamation,
//     question,
//     greaterthan,
//     lessthan,
//     forwardslash,
//     backslash,
//     at,
//     vbar,
//     lbrace,rbrace,
//     lbracket,rbracket,
//     colon,
//     semicolon,
//     squote,
//     dquote,
//     pound,
//     dollar,
//     percent,
//     circumflex,
//     ampersand,
//     asterisk,
//     lparen,
//     rparen,
//     minus,
//     underscore,
//     plus,
//     equals,
//     del,
//     grave,
//     tilde
// };

typedef struct {
    // KeyCode kc;
     OmegaWTK::Unicode32Char key;
} KeyDownParams;

typedef struct {
    OmegaWTK::Unicode32Char key;
} KeyUpParams;


class NativeEventProcessor;
//
class OMEGAWTK_EXPORT NativeEventEmitter {
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
class OMEGAWTK_EXPORT NativeEventProcessor {
    public:
    virtual void onRecieveEvent(NativeEventPtr event) = 0;
    NativeEventProcessor();
    ~NativeEventProcessor();
};
    
};
};

#endif
