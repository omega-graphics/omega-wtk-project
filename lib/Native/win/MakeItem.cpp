#include "MakeItem.h"
#include "NativePrivate/win/HWNDItem.h"

namespace OmegaWTK::Native::Win {
    NativeItemPtr make_item_win(Core::Rect & rect,ItemType type){
        HWNDItem::Type win_type = HWNDItem::View;
        if(type == Native::Default){
            win_type = HWNDItem::View;
        };
        return new HWNDItem(rect,win_type);
    };
};