#include "MakeItem.h"
#include "NativePrivate/win/HWNDItem.h"
#include "WinAppWindow.h"
namespace OmegaWTK::Native::Win {
    NativeItemPtr make_item_win(Core::Rect & rect,ItemType type,NativeItemPtr parent){
        HWNDItem::Type win_type = HWNDItem::View;
        if(type == Native::Default){
            win_type = HWNDItem::View;
        };
        return new HWNDItem(rect,win_type,(HWNDItem *)parent);
    };
    NWH make_win_window(Core::Rect &rect){
       return new WinAppWindow(rect);
    };
};