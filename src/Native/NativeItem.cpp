#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"

//#ifdef TARGET_WIN32
//#include "win/MakeItem.h"
//#endif
//
//#ifdef TARGET_MACOS
//#include "macos/MakeItem.h"
//#endif

namespace OmegaWTK::Native {
   
    NativeItem::NativeItem():event_emitter(nullptr){};

    bool NativeItem::hasEventEmitter(){
        return event_emitter != nullptr;
    };

    void NativeItem::sendEventToEmitter(NativeEventPtr event){
        event_emitter->emit(event);
    };

    void set_native_item_event_emitter(NativeItemPtr ptr,NativeEventEmitter * emitter){
        ptr->event_emitter = emitter;
    };
};
