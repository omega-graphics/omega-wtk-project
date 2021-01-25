#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"


namespace OmegaWTK::Native::Cocoa {

void ns_string_to_core_string(NSString *str,Core::String & res){
    res.assign(str.UTF8String);
};

NSString * core_string_to_ns_string(Core::String & str){
    return [[NSString alloc] initWithData:[[NSData alloc] initWithBytes:str.data() length:str.size()] encoding:NSUTF8StringEncoding];
};

NSRect core_rect_to_cg_rect(Core::Rect & rect){
    return NSMakeRect(rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight);
};

NativeItemPtr make_item_cocoa(Core::Rect & rect,OPT_PARAM type){
    OmegaWTKCocoaView *ptr;
    OPT_PARAM item_type;
    ptr = [[OmegaWTKCocoaView alloc] initWithFrame:core_rect_to_cg_rect(rect)];
    item_type = CocoaItem::View;
    return new CocoaItem(ptr,item_type);
};

};

