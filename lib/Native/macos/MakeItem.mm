#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"
#include "omegaWTK/Native/NativeEvent.h"

#import "CocoaAppWindow.h"


namespace OmegaWTK::Native::Cocoa {

NativeItemPtr make_item_cocoa(const Core::Rect & rect,ItemType type,NativeItemPtr parent){
    CocoaItem::Type item_type;
    if(type == Default)
        item_type = CocoaItem::View;
    else if(type == ScrollItem){
        item_type = CocoaItem::ScrollView;
    };
    return new CocoaItem(rect,item_type,(CocoaItem *)parent);
};

NWH make_cocoa_window(Core::Rect & rect,NativeEventEmitter *emitter){
    return new CocoaAppWindow(rect,emitter);
};

};

