#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"


namespace OmegaWTK::Native::Cocoa {

NSRect core_rect_to_cg_rect(Core::Rect & rect){
    return NSMakeRect(rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight);
};

NativeItemPtr make_item_cocoa(Core::Rect & rect,OPT_PARAM type){
    NSView *ptr;
    OPT_PARAM item_type;
    if(type == Native::Default){
        ptr = [[NSView alloc] initWithFrame:core_rect_to_cg_rect(rect)];
        item_type = CocoaItem::View;
    }
    else if(type == Native::PushButton){
        ptr = [[NSButton alloc] initWithFrame:core_rect_to_cg_rect(rect)];
        item_type = CocoaItem::Button;
    };
    return new CocoaItem(ptr,item_type);
};

};

