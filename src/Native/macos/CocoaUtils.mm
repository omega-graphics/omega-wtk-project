#import "NativePrivate/macos/CocoaUtils.h"

namespace OmegaWTK::Native::Cocoa {

void ns_string_to_common_string(NSString *str,OmegaCommon::String & res){
    res.assign(str.UTF8String);
};

NSString * common_string_to_ns_string(const OmegaCommon::String & str){
    return [[NSString alloc] initWithData:[[NSData alloc] initWithBytes:str.data() length:str.size()] encoding:NSUTF8StringEncoding];
};

NSRect core_rect_to_cg_rect(const Core::Rect & rect){
    std::cout << "X:" << rect.pos.x  << "Y:" << rect.pos.y << "W:" << rect.w << "H:" << rect.h;
    return NSMakeRect(rect.pos.x,rect.pos.y,rect.w,rect.h);
};



CGPoint core_pos_to_cg_point(const Core::Position & pos){
    return CGPointMake(pos.x,pos.y);
};

};
