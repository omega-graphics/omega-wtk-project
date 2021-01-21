#include "MakeItem.h"
#include "NativePrivate/win/HWNDItem.h"

namespace OmegaWTK::Native::Win {
    NativeItemPtr make_item_win(Core::Dimensions dimen,OPT_PARAM type){
        return new HWNDItem();
    };
};