#include "MakeItem.h"
#include "NativePrivate/win/HWNDItem.h"
#include "NativePrivate/win/HWNDFactory.h"

namespace OmegaWTK::Native::Win {
    NativeItemPtr make_item_win(Core::Rect & rect,OPT_PARAM type){
        HWND hwnd;
        OPT_PARAM win_type;
        if(type == Native::Default){
            hwnd = HWNDFactory::appFactoryInst->makeWindow("","",rect,WS_CHILD,nullptr);
            win_type = HWNDItem::View;
        }
        else if(type == Native::PushButton){
            hwnd = HWNDFactory::appFactoryInst->makeWindow("BUTTON", "",rect,WS_CHILD | BS_DEFPUSHBUTTON,nullptr);
            win_type = HWNDItem::PushButton;
        }
        else {
            hwnd = nullptr;
            win_type = HWNDItem::View;
        };
        return new HWNDItem(hwnd,win_type);
    };
};