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
        self.layer.backgroundColor = [NSColor blueColor].CGColor;
        self.layer.masksToBounds = YES;
        self.layer.frame = rect;
        self.layer.bounds = CGRectMake(0.0,0.0,rect.size.width,rect.size.height);
        self.layer.autoresizingMask = kCALayerHeightSizable | kCALayerWidthSizable;
        NSLog(@"Old Origin: { x:%f, y:%f}",self.layer.anchorPoint.x,self.layer.anchorPoint.y);
        self.layer.anchorPoint = CGPointMake(0.0,0.0);
        self.layer.position = rect.origin;
        self.layer.contentsScale = 1.0f;
        NSLog(@"New Origin: { x:%f, y:%f}",self.layer.anchorPoint.x,self.layer.anchorPoint.y);
        _delegate = delegate;
    };
    return self;
};
-(void)emitEventIfPossible:(NSEvent *)event{
    if(self.delegate->hasEventEmitter()){
        self.delegate->sendEventToEmitter(OmegaWTK::Native::Cocoa::ns_event_to_omega_wtk_native_event(event));
    };
};

- (BOOL)acceptsFirstResponder {
    return YES;
};

- (void)mouseDown:(NSEvent *)event{
    [self emitEventIfPossible:event];
    [super mouseDown:event];
};
- (void)mouseUp:(NSEvent *)event{
    [self emitEventIfPossible:event];
    [super mouseUp:event];
};
- (void)mouseEntered:(NSEvent *)event{
    [self emitEventIfPossible:event];
    [super mouseEntered:event];
};
- (void)mouseExited:(NSEvent *)event {
    [self emitEventIfPossible:event];
    [super mouseExited:event];
};

- (void)drawRect:(NSRect)dirtyRect{
    NSLog(@"NEVER CALL THIS FUNCTION!!!");
};
- (CALayer *)getCALayer {
    return (CALayer *)self.layer;
};

@end

@implementation OmegaWTKCocoaViewController{
    OmegaWTK::Native::Cocoa::CocoaItem *_delegate;
    NSRect _rect;
}
- (instancetype)initWithFrame:(NSRect)rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *)delegate{
    if(self = [super init]){
        _rect = rect;
        _delegate = delegate;
    };
    return self;
};

- (void)loadView{
    NSLog(@"Load the View!");
    self.view = [[OmegaWTKCocoaView alloc] initWithFrame:_rect delegate:_delegate];
    self.view.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
};

- (void)viewDidLoad{
    NSLog(@"View Has Loaded.. Emitting Event!");
    _delegate->isReady = true;
};

- (void)viewDidLayout{
    [self.view setFrame:OmegaWTK::Native::Cocoa::core_rect_to_cg_rect(_delegate->rect)];
    [self.view setBounds:CGRectMake(0.0,0.0,_delegate->rect.dimen.minWidth,_delegate->rect.dimen.minHeight)];
    self.view.layer.frame = self.view.frame;
    self.view.layer.bounds = self.view.bounds;
    _delegate->layoutLayerTreeLimb();
};

@end

namespace OmegaWTK::Native::Cocoa {

CocoaItem::CocoaItem(const Core::Rect & rect,CocoaItem::Type _type):rect(rect),type(_type),isReady(false){
    cont = [[OmegaWTKCocoaViewController alloc] initWithFrame:core_rect_to_cg_rect(rect) delegate:this];
    _ptr = (OmegaWTKCocoaView *)cont.view;
};

void CocoaItem::layoutLayerTreeLimb(){
    layerTreelimb->layout();
};

void * CocoaItem::getBinding(){
    return reinterpret_cast<void *>(this->cont);
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
