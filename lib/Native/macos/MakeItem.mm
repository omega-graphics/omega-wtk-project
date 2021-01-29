#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"


namespace OmegaWTK::Native::Cocoa {

NativeItemPtr make_item_cocoa(Core::Rect & rect,OPT_PARAM type){
    OmegaWTKCocoaView *ptr;
    CocoaItem::Type item_type;
    ptr = [[OmegaWTKCocoaView alloc] initWithFrame:core_rect_to_cg_rect(rect)];
    item_type = CocoaItem::View;
    return new CocoaItem(ptr,item_type);
};

};

