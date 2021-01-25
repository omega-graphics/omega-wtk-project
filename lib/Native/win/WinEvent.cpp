#include "NativePrivate/win/WinEvent.h"
#include "NativePrivate/win/HWNDItem.h"

namespace OmegaWTK::Native::Win {
    NativeEventPtr create_event_ptr(NativeEvent::EventType _Ty){
        return new NativeEvent(_Ty);
    };

    WinEventHandler::WinEventHandler(NativeItemPtr ptr):item((HWNDItem *)ptr){};

    NativeEventCallback WinEventHandler:: getCallback(NativeEvent::EventType type){
        auto it = listeners.find(type);
        if(it != listeners.end()){
            return (*it).second;
        };
        return nullptr;
    };

    LRESULT WinEventHandler::handleMessages(UINT msg,WPARAM wparam,LPARAM lparam){
        if(isListening){
            switch (msg) {
                case WM_LBUTTONDOWN : {
                    auto callback = getCallback(NativeEvent::Click);
                    callback(create_event_ptr(NativeEvent::Click));
                    break;
                }
                case WM_MOUSEHOVER : {
                    auto callback = getCallback(NativeEvent::Hover);
                    callback(create_event_ptr(NativeEvent::Hover));
                    break;
                }
                    
            }   
        }
        else {
            return 0;
        };
        
    };
};