#include "NativePrivate/win/WinEvent.h"

namespace OmegaWTK::Native {
NativeEventPtr button_event_to_native_event(NativeEvent::EventType event_type,LPPOINT pt){
    NativeEventParams params;
    switch (event_type) {
    case NativeEvent::CursorEnter : {
        params = new CursorEnterParams();
        break;
    }
    case NativeEvent::CursorExit : {
        params = new CursorExitParams();
        break;
    };
    case NativeEvent::LMouseDown : {
        params = new LMouseDownParams();
        break;
    };
    case NativeEvent::LMouseUp : {
        params = new LMouseUpParams();
        break;
    }
    default : {
        params = nullptr;
        break;
    }
    }
    return new NativeEvent(event_type,params);
};
}