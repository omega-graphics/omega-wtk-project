#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"


namespace OmegaWTK::Native::Cocoa {

NativeItemPtr make_item_cocoa(const Core::Rect & rect,OPT_PARAM type){
    CocoaItem::Type item_type;
    item_type = CocoaItem::View;
    return new CocoaItem(rect,item_type);
};

};

