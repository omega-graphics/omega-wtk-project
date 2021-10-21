#import "NativePrivate/macos/CocoaEvent.h"
#import "NativePrivate/macos/CocoaItem.h"
#include "omegaWTK/Core/Unicode.h"

namespace OmegaWTK::Native::Cocoa {

    NativeEventPtr ns_event_to_omega_wtk_native_event(NSEvent *event){
#define NS_EVENT_TYPE_CASE(name,args...) case name : { args break;};
#define SET_NATIVE_EVENT_PARAMS(var,value) var = reinterpret_cast<NativeEventParams>(value)
        NativeEvent::EventType type;
        NativeEventParams params;
        #define KEY_CASE(ch,och) case ch: \
                                        omegaKC = och;\
                                        break;

        // #define EVAL_KEYS() KeyCode omegaKC; \
        //                         \
        //                         switch(kc){\
        //                             KEY_CASE('a',a)\
        //                             KEY_CASE('b',b)\
        //                             KEY_CASE('c',c)\
        //                             KEY_CASE('d',d)\
        //                             KEY_CASE('e',e)\
        //                             KEY_CASE('f',f)\
        //                         };
        switch(event.type){
            NS_EVENT_TYPE_CASE(NSEventTypeMouseEntered,
                                NSLog(@"Mouse Enter NSEvent %@",event);
                               type = NativeEvent::EventType::CursorEnter;
                               SET_NATIVE_EVENT_PARAMS(params,new CursorEnterParams());
                               
                               )
            NS_EVENT_TYPE_CASE(NSEventTypeMouseExited,
                                NSLog(@"Mouse Exit NSEvent %@",event);
                               type = NativeEvent::EventType::CursorExit;
                               SET_NATIVE_EVENT_PARAMS(params,new CursorExitParams());
                               
                               )
            NS_EVENT_TYPE_CASE(NSEventTypeLeftMouseDown,
                               type = NativeEvent::EventType::LMouseDown;
                               SET_NATIVE_EVENT_PARAMS(params,new LMouseDownParams());
                               )
            NS_EVENT_TYPE_CASE(NSEventTypeLeftMouseUp,
                               type = NativeEvent::EventType::LMouseUp;
                               SET_NATIVE_EVENT_PARAMS(params,new LMouseUpParams());
                               
                               )
            NS_EVENT_TYPE_CASE(NSEventTypeKeyDown,
                                type = NativeEvent::EventType::KeyDown;
                                auto kc = event.keyCode;
                                OmegaWTK::Unicode32Char omegaKC = kc;
                                // EVAL_KEYS()
                               SET_NATIVE_EVENT_PARAMS(params,new KeyDownParams({omegaKC}));
                                )
            NS_EVENT_TYPE_CASE(NSEventTypeKeyUp,
                                type = NativeEvent::EventType::KeyUp;
                                auto kc = event.keyCode;
                                OmegaWTK::Unicode32Char omegaKC = kc;
                                // EVAL_KEYS()
                               SET_NATIVE_EVENT_PARAMS(params,new KeyUpParams({omegaKC}));
                                )
            default: {
                type = NativeEvent::Unknown;
                params = nullptr;
                break;
            };
        }
        return (NativeEventPtr)new OmegaWTK::Native::NativeEvent(type,params);
    };

};
