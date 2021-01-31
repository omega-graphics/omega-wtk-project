#import "NativePrivate/macos/CocoaEvent.h"
#import "NativePrivate/macos/CocoaItem.h"

namespace OmegaWTK::Native::Cocoa {

    NativeEventPtr ns_event_to_omega_wtk_native_event(NSEvent *event){
#define NS_EVENT_TYPE_CASE(name,args...) case name : { args break;};
#define SET_NATIVE_EVENT_PARAMS(var,value) var = reinterpret_cast<NativeEventParams>(value)
        NativeEvent::EventType type;
        NativeEventParams params;
        switch(event.type){
            NS_EVENT_TYPE_CASE(NSEventTypeMouseEntered,
                               type = NativeEvent::EventType::CursorEnter;
                               SET_NATIVE_EVENT_PARAMS(params,new CursorEnterParams());
                               
                               )
            NS_EVENT_TYPE_CASE(NSEventTypeMouseExited,
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
            default: {
                break;
            };
        }
        return new OmegaWTK::Native::NativeEvent(type,params);
    };

};
