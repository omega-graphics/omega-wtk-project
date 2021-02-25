#import "NativePrivate/macos/CocoaItem.h"
#import "NativePrivate/macos/CocoaEvent.h"
#import "NativePrivate/macos/CocoaUtils.h"

#import <QuartzCore/QuartzCore.h>

@interface OmegaWTKCocoaView ()
@property (nonatomic) OmegaWTK::Native::Cocoa::CocoaItem *delegate;
@end

@implementation OmegaWTKCocoaView
- (instancetype)initWithFrame:(NSRect)rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *)delegate{
    if(self = [super initWithFrame:rect]){
        self.wantsLayer = YES;
        self.layer = [CALayer layer];
        self.layer.masksToBounds = NO;
        _delegate = delegate;
    };
    return self;
};
-(void)emitEventIfPossible:(NSEvent *)event{
    if(self.delegate->hasEventEmitter()){
        self.delegate->sendEventToEmitter(OmegaWTK::Native::Cocoa::ns_event_to_omega_wtk_native_event(event));
    };
};

- (void)mouseDown:(NSEvent *)event{
    [self emitEventIfPossible:event];
};
- (void)mouseUp:(NSEvent *)event{
    [self emitEventIfPossible:event];
};
- (void)mouseEntered:(NSEvent *)event{
    [self emitEventIfPossible:event];
};
- (void)mouseExited:(NSEvent *)event {
    [self emitEventIfPossible:event];
};

- (void)drawRect:(NSRect)dirtyRect{
    NSLog(@"NEVER CALL THIS FUNCTION!!!");
};
- (CALayer *)getCALayer {
    return (CALayer *)self.layer;
};

@end

namespace OmegaWTK::Native::Cocoa {

CocoaItem::CocoaItem(const Core::Rect & rect,CocoaItem::Type _type):rect(rect),type(_type){
    _ptr = [[OmegaWTKCocoaView alloc] initWithFrame:OmegaWTK::Native::Cocoa::core_rect_to_cg_rect(rect) delegate:this];
};

void * CocoaItem::getBinding(){
    return reinterpret_cast<void *>(_ptr);
};

void CocoaItem::enable(){
    if([_ptr isHidden] == YES)
        [_ptr setHidden:NO];
};

void CocoaItem::disable(){
    if([_ptr isHidden] == NO)
        [_ptr setHidden:YES];
};

void CocoaItem::addChildNativeItem(NativeItemPtr native_item){
    OmegaWTKCocoaView *cocoaview = (OmegaWTKCocoaView *)native_item;
    [_ptr addSubview:cocoaview];
};

void CocoaItem::removeChildNativeItem(NativeItemPtr native_item){
    [_ptr replaceSubview:(OmegaWTKCocoaView *)native_item with:[[NSView alloc] initWithFrame:NSZeroRect]];
};

CocoaItem::~CocoaItem(){
    
};

void CocoaItem::setNeedsDisplay(){
    _ptr.needsDisplay = YES;
};

};
