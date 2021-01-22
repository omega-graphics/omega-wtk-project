#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_NATIVE_WIN_MAKEITEM_H
#define OMEGAWTK_NATIVE_WIN_MAKEITEM_H

namespace OmegaWTK::Native {
    namespace Win {
        NativeItemPtr make_item_win(Core::Rect & dimen,OPT_PARAM type);
    };
};

#endif