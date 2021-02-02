#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"


namespace OmegaWTK::Native::Cocoa {

NativeItemPtr make_item_cocoa(const Core::Rect & rect){
    CocoaItem::Type item_type;
    item_type = CocoaItem::View;
    return new CocoaItem(rect,item_type);
};

};

