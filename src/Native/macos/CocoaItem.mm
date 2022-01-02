#import "NativePrivate/macos/CocoaItem.h"
#import "NativePrivate/macos/CocoaEvent.h"
#import "NativePrivate/macos/CocoaUtils.h"

#import <QuartzCore/QuartzCore.h>

@interface OmegaWTKCocoaView ()
@property (nonatomic) OmegaWTK::Native::Cocoa::CocoaItem *delegate;
@property(nonatomic,retain) NSTrackingArea *trackingArea;
@end


@implementation OmegaWTKCocoaView
- (instancetype)initWithFrame:(NSRect)rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *)delegate{
    if(self = [super initWithFrame:rect]){
        self.wantsLayer = YES;
        self.layer = [CALayer layer];
        self.layer.masksToBounds = YES;
        self.layer.bounds = rect;
        self.autoresizesSubviews = NO;
         self.layer.autoresizingMask = kCALayerHeightSizable | kCALayerWidthSizable;
        NSLog(@"Old Origin: { x:%f, y:%f}",self.layer.anchorPoint.x,self.layer.anchorPoint.y);
        self.layer.anchorPoint = CGPointMake(0.0,0.0);
        self.layer.position = rect.origin;
        self.layer.contentsScale = 1.0f;
        // self.layer.contentsGravity = kCAGravityCenter;
        self.layer.magnificationFilter = kCAFilterLinear;
//        self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;
//        self.layerContentsPlacement = NSViewLayerContentsPlacementCenter;
        NSLog(@"New Origin: { x:%f, y:%f}",self.layer.anchorPoint.x,self.layer.anchorPoint.y);
        _delegate = delegate;
        _trackingArea = [[NSTrackingArea alloc] initWithRect:rect options:NSTrackingMouseEnteredAndExited | NSTrackingCursorUpdate | NSTrackingActiveInKeyWindow owner:self userInfo:nil];
        [self addTrackingArea:_trackingArea];
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
-(BOOL)acceptsFirstMouse:(NSEvent *)event {
    return YES;
}
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
    Class _class;
    NSRect _rect;
}
- (instancetype)initWithFrame:(NSRect)rect delegate:(OmegaWTK::Native::Cocoa::CocoaItem *)delegate{
    if(self = [super init]){
        _rect = rect;
        _delegate = delegate;
    };
    return self;
};

-(void)setClass:(Class)cls {
    _class = cls;
}

- (void)loadView{
    NSLog(@"Load the View!");
    if(_class == [OmegaWTKCocoaView class]){
        self.view = [[_class alloc] initWithFrame:_rect delegate:_delegate];
    }
    else {
        self.view = [[_class alloc] initWithFrame:_rect];
    }
    self.view.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
};

- (void)viewDidLoad{
    NSLog(@"View Delegate is Ready!");
    _delegate->isReady = true;
};

// - (void)viewDidLayout{
//     [self.view setFrame:OmegaWTK::Native::Cocoa::core_rect_to_cg_rect(_delegate->rect)];
//     [self.view setBounds:CGRectMake(0.0,0.0,_delegate->rect.dimen.minWidth,_delegate->rect.dimen.minHeight)];
//     self.view.layer.position = self.view.frame.origin;
//     self.view.layer.frame = self.view.frame;
//     self.view.layer.bounds = self.view.bounds;
//     _delegate->layoutLayerTreeLimb();
// };

@end

@interface OmegaWTKCocoaScrollViewDelegate ()
-(instancetype)init;
-(void)onScrollLeft;
-(void)onScrollRight;
-(void)onScrollUp;
-(void)onScrollDown;
@end

namespace OmegaWTK::Native::Cocoa {

CocoaItem::CocoaItem(const Core::Rect & rect,CocoaItem::Type _type,SharedHandle<CocoaItem> parent):rect(rect),type(_type),isReady(false){
    if(type == View){
        cont = [[OmegaWTKCocoaViewController alloc] initWithFrame:core_rect_to_cg_rect(rect) delegate:this];
        [cont setClass:[OmegaWTKCocoaView class]];
        _ptr = (OmegaWTKCocoaView *)cont.view;
        scrollView = nil;
        if(parent != nullptr){
            parent->addChildNativeItem((NativeItemPtr)this);
        };
    };
    if(type == ScrollView){
        _ptr = nil;
        cont = [[OmegaWTKCocoaViewController alloc] initWithFrame:core_rect_to_cg_rect(rect) delegate:this];
        [cont setClass:[NSScrollView class]];
        scrollViewDelegate = [[OmegaWTKCocoaScrollViewDelegate alloc] init];
        scrollView = (NSScrollView *)cont.view;
        scrollView.autohidesScrollers = YES;
        scrollView.borderType = NSNoBorder;

        [scrollView addObserver:scrollViewDelegate forKeyPath:@"horizontalLineScroll" options:NSKeyValueObservingOptionNew context:nil];
        [scrollView addObserver:scrollViewDelegate forKeyPath:@"verticalLineScroll" options:NSKeyValueObservingOptionNew context:nil];
        if(parent != nullptr){
            parent->addChildNativeItem((NativeItemPtr)this);
        };
    };
};

// void CocoaItem::layoutLayerTreeLimb(){
//     layerTreelimb->layout();
// };

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

void CocoaItem::resize(const Core::Rect &newRect){
    rect = newRect;
    CGRect r = core_rect_to_cg_rect(newRect);
    if(_ptr != nil){
        [_ptr setFrame:r];
        [_ptr setBoundsOrigin:NSMakePoint(0,0)];
        [_ptr setBoundsSize:r.size];
        CALayer *layer = _ptr.layer;
        layer.frame = r;
        layer.position = _ptr.frame.origin;
        layer.bounds = _ptr.bounds;
    }
    else {
        [scrollView setFrame:r];
    }
     
};

void CocoaItem::addChildNativeItem(NativeItemPtr native_item){
    auto cocoaview = std::dynamic_pointer_cast<CocoaItem>(native_item);
    if(cocoaview->_ptr != nil){
        [_ptr addSubview:cocoaview->_ptr];
    }
    else if(cocoaview->scrollView != nil){
        [_ptr addSubview:cocoaview->scrollView];
    }
};

void CocoaItem::removeChildNativeItem(NativeItemPtr native_item){
    auto cocoaview = std::dynamic_pointer_cast<CocoaItem>(native_item);
    if(cocoaview->_ptr != nil){
        [cocoaview->_ptr removeFromSuperview];
    }
    else if(cocoaview->scrollView != nil){
        [cocoaview->scrollView removeFromSuperview];
    }
};

void CocoaItem::setClippedView(NativeItemPtr nativeItem){
    auto cocoaItem = std::dynamic_pointer_cast<CocoaItem>(nativeItem);
    scrollView.documentView = cocoaItem->_ptr;
};

void CocoaItem::toggleHorizontalScrollBar(bool &state){
    if(state)
        [scrollView setHasHorizontalScroller:YES];
    else 
        [scrollView setHasHorizontalScroller:NO];
};

void CocoaItem::toggleVerticalScrollBar(bool &state){
    if(state)
        [scrollView setHasVerticalScroller:YES];
    else 
        [scrollView setHasVerticalScroller:NO];
};

CocoaItem::~CocoaItem(){
    
};

void CocoaItem::setNeedsDisplay(){
    _ptr.needsDisplay = YES;
};



};

@implementation OmegaWTKCocoaScrollViewDelegate
- (void)didChangeValueForKey:(NSString *)key {

}
@end

namespace OmegaWTK::Native {
    NativeItemPtr make_native_item(Core::Rect rect,ItemType type,NativeItemPtr parent){
        Cocoa::CocoaItem::Type item_type;
        if(type == Default)
            item_type = Cocoa::CocoaItem::View;
        else if(type == ScrollItem){
            item_type = Cocoa::CocoaItem::ScrollView;
        }
        else {
            item_type = Cocoa::CocoaItem::View;
        }
        return (NativeItemPtr)new Cocoa::CocoaItem(rect,item_type,std::dynamic_pointer_cast<Cocoa::CocoaItem>(parent));
    };
}
