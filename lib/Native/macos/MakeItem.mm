#include "MakeItem.h"
#include "NativePrivate/macos/CocoaItem.h"
#include "NativePrivate/macos/CocoaUtils.h"
#include "omegaWTK/Native/NativeEvent.h"

#import "CocoaAppWindow.h"


namespace OmegaWTK::Native::Cocoa {

NativeItemPtr make_item_cocoa(const Core::Rect & rect){
    CocoaItem::Type item_type;
    item_type = CocoaItem::View;
    return new CocoaItem(rect,item_type);
};

NWH make_cocoa_window(Core::Rect & rect,NativeEventEmitter *emitter){
    return new CocoaAppWindow(rect,emitter);
};

};

