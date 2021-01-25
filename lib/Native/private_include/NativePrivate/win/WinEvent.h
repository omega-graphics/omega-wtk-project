#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeEvent.h"

#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_WINEVENT_H
#define OMEGAWTK_NATIVE_WIN_WINEVENT_H

namespace OmegaWTK::Native {
    namespace Win {
        class HWNDItem;

        class WinEventHandler : public NativeEventHandler {
            HWNDItem * item;
            bool isListening = false;
            NativeEventCallback getCallback(NativeEvent::EventType type);
            public:
            WinEventHandler(NativeItemPtr ptr);
            LRESULT handleMessages(UINT msg,WPARAM wparam,LPARAM lparam);
            private:
            void listen(){
                isListening = true;
            };
        };
    };
};

#endif