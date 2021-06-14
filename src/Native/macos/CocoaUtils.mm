#import "NativePrivate/macos/CocoaUtils.h"

namespace OmegaWTK::Native::Cocoa {

void ns_string_to_core_string(NSString *str,Core::String & res){
    res.assign(str.UTF8String);
};

NSString * core_string_to_ns_string(const Core::String & str){
    return [[NSString alloc] initWithData:[[NSData alloc] initWithBytes:str.data() length:str.size()] encoding:NSUTF8StringEncoding];
};

NSRect core_rect_to_cg_rect(const Core::Rect & rect){
    return NSMakeRect(rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight);
};

CGPoint core_pos_to_cg_point(const Core::Position & pos){
    return CGPointMake(pos.x,pos.y);
};

};
