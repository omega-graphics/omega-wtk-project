#import "NativePrivate/macos/CocoaEvent.h"

void ns_event_to_omega_wtk_native_event(NSEvent *event,OmegaWTK::Native::NativeEventPtr res){
    OmegaWTK::Core::Option type;
    if(event.type == NSEventTypeMouseEntered){
        type = OmegaWTK::Native::NativeEvent::Hover;
    }
    else if(event.type == NSEventTypeLeftMouseDown){
        type = OmegaWTK::Native::NativeEvent::Click;
    };
    res = new OmegaWTK::Native::NativeEvent(type);
};

@interface OmegaWTKCocoaEventDelegate ()
@property (nonatomic) OmegaWTK::Native::NativeEventListenerMap & map;
@end

@implementation OmegaWTKCocoaEventDelegate

-(OmegaWTK::Native::NativeEventCallback &) getCallback:(OmegaWTK::Core::Option) opt {
    auto & listeners = self.map;
    auto f_pos = listeners.find(opt);
    if(f_pos != listeners.end()){
        return (*f_pos).second;
    };
    
};

- (instancetype)initWithItem:(OmegaWTKCocoaView *)item callbacks:(OmegaWTK::Native::NativeEventListenerMap &)__map{
    if(self = [super init]){
        _map = __map;
        [item setDelegate:self];
    };
    return self;
};
-(void)onClick:(NSEvent *)event{
    OmegaWTK::Native::NativeEventPtr ptr;
    ns_event_to_omega_wtk_native_event(event,ptr);
    auto callback = [self getCallback:OmegaWTK::Native::NativeEvent::Click];
    callback(ptr);
};
-(void)onHover:(NSEvent *)event{
    OmegaWTK::Native::NativeEventPtr ptr;
    ns_event_to_omega_wtk_native_event(event,ptr);
    auto callback = [self getCallback:OmegaWTK::Native::NativeEvent::Hover];
    callback(ptr);
};

@end

namespace OmegaWTK::Native::Cocoa {

CocoaEventHandler::CocoaEventHandler(NativeItemPtr ptr):item((CocoaItem *)ptr){
    
};

void CocoaEventHandler::listen() {
    native_event_delegate = [[OmegaWTKCocoaEventDelegate alloc] initWithItem:item->_ptr callbacks:listeners];
};

};
