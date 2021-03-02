#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeWindow.h"

#ifndef OMEGAWTK_NATIVE_WIN_MAKEITEM_H
#define OMEGAWTK_NATIVE_WIN_MAKEITEM_H

namespace OmegaWTK::Native {
    namespace Win {
        NativeItemPtr make_item_win(Core::Rect & dimen,ItemType type,NativeItemPtr parent);
        NWH make_win_window(Core::Rect & rect,NativeEventEmitter *emitter);
    };
};

#endif