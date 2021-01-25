#import "NativePrivate/macos/CocoaItem.h"
#import "NativePrivate/macos/CocoaEvent.h"

@interface OmegaWTKCocoaEventDelegate ()
-(void)onClick:(NSEvent *)event;
-(void)onHover:(NSEvent *)event;
@end

@interface OmegaWTKCocoaView ()
@property (nonatomic,retain) OmegaWTKCocoaEventDelegate *delegate;
@end

@implementation OmegaWTKCocoaView

-(BOOL) delegateIsReady{
    return self.delegate != nil;
};

-(void)setDelegate:(OmegaWTKCocoaEventDelegate *)delegate{
    self.delegate = delegate;
};
- (void)mouseDown:(NSEvent *)event{
    if([self delegateIsReady])
        [self.delegate onClick:event];
};
- (void)mouseEntered:(NSEvent *)event{
    if([self delegateIsReady])
        [self.delegate onHover:event];
};

@end

namespace OmegaWTK::Native::Cocoa {

CocoaItem::CocoaItem(OmegaWTKCocoaView *ptr,OPT_PARAM _type):_ptr(ptr),type(_type){
    
};

CocoaItem::~CocoaItem(){
    
};

};
