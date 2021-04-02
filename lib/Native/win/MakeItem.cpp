#include "MakeItem.h"
#include "NativePrivate/win/HWNDItem.h"
#include "WinAppWindow.h"
namespace OmegaWTK::Native::Win {
    NativeItemPtr make_item_win(Core::Rect & rect,ItemType type,NativeItemPtr parent){
        HWNDItem::Type win_type = HWNDItem::View;
        if(type == Native::Default){
            win_type = HWNDItem::View;
        }
        else if(type == Native::ScrollItem){
            win_type = HWNDItem::ScrollView;
        };
        return new HWNDItem(rect,win_type,(HWNDItem *)parent);
    };
    NWH make_win_window(Core::Rect &rect,NativeEventEmitter *emitter){
       return new WinAppWindow(rect,emitter);
    };
};