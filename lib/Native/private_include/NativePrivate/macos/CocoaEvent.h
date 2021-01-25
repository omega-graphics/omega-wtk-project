#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeEvent.h"
#import "CocoaItem.h"

#import <Cocoa/Cocoa.h>

#ifndef OMEGAWTK_NATIVE_COCOAEVENT_H
#define OMEGAWTK_NATIVE_COCOAEVENT_H

@interface OmegaWTKCocoaEventDelegate : NSObject
-(instancetype) initWithItem:(OmegaWTKCocoaView *) item callbacks:(OmegaWTK::Native::NativeEventListenerMap &) __map;
@end

namespace OmegaWTK::Native {
namespace Cocoa {

class CocoaEventHandler : public NativeEventHandler {
    CocoaItem *item;
    OmegaWTKCocoaEventDelegate *native_event_delegate;
public:
    CocoaEventHandler(NativeItemPtr ptr);
private:
    void listen();
};

};
}

#endif
