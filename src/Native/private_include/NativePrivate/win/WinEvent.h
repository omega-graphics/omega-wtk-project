#include <Windows.h>
#include "omegaWTK/Native/NativeEvent.h"

#ifndef OMEGAWTK_NATIVE_WIN_WINEVENT_H
#define OMEGAWTK_NATIVE_WIN_WINEVENT_H

namespace OmegaWTK::Native {

NativeEventPtr button_event_to_native_event(NativeEvent::EventType event_type,LPPOINT pt);

};

#endif