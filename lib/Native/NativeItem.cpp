#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"

#ifdef TARGET_WIN32
#include "win/MakeItem.h" 
#endif

#ifdef TARGET_MACOS
#include "macos/MakeItem.h"
#endif

namespace OmegaWTK::Native {
   
    NativeItem::NativeItem():event_emitter(nullptr){};

    bool NativeItem::hasEventEmitter(){
        return event_emitter != nullptr;
    };

    void NativeItem::sendEventToEmitter(NativeEventPtr event){
        event_emitter->emit(event);
    };

    NativeItemPtr make_native_item(Core::Rect rect,ItemType type){
        #ifdef TARGET_WIN32
        return Win::make_item_win(rect,type); 
        #endif
        #ifdef TARGET_MACOS
        return Cocoa::make_item_cocoa(rect);
        #endif
    };

    void set_native_item_event_emitter(NativeItemPtr ptr,NativeEventEmitter * emitter){
        ptr->event_emitter = emitter;
    };
};
