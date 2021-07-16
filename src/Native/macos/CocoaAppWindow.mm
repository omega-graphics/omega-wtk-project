#import "CocoaAppWindow.h"
#include "NativePrivate/macos/CocoaUtils.h"
#include "NativePrivate/macos/CocoaItem.h"


@interface OmegaWTKNativeCocoaAppWindowDelegate : NSObject <NSWindowDelegate>
@property(nonatomic,strong) NSWindow *window;
@property(nonatomic) OmegaWTK::Native::Cocoa::CocoaAppWindow * cppBinding;
@end

@interface OmegaWTKNativeCocoaAppWindowController : NSWindowController
- (instancetype)initWithRect:(NSRect) rect delegate:(id<NSWindowDelegate>) delegate;
@end

namespace OmegaWTK::Native::Cocoa {


CocoaAppWindow::CocoaAppWindow(Core::Rect & rect,NativeEventEmitter *emitter):NativeWindow(rect){
    eventEmitter = emitter;

    windowDelegate = [[OmegaWTKNativeCocoaAppWindowDelegate alloc] init];
    windowController = [[OmegaWTKNativeCocoaAppWindowController alloc] initWithRect:core_rect_to_cg_rect(rect) delegate:windowDelegate];
    windowDelegate.cppBinding = this; 
    NSView *rootView = [[NSView alloc] initWithFrame:windowController.window.frame];
    rootView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [windowController.window setContentView:rootView];
};

NativeEventEmitter * CocoaAppWindow::getEmitter() {
    return eventEmitter;
};

void CocoaAppWindow::disable(){
    if([windowController.window isVisible] == YES){
        [windowController.window orderOut:nil];
    };
};

void CocoaAppWindow::enable(){
    if([windowController.window isVisible] == NO){
        [windowController.window makeKeyAndOrderFront:nil];
    };
};

void CocoaAppWindow::addNativeItem(NativeItemPtr item){
        // auto *cocoaitem = (CocoaItem *)item;
        NSViewController *viewC = (NSViewController *)item->getBinding();
        [windowController.window.contentView addSubview:viewC.view];
};

// void CocoaAppWindow::attachWidgets(){
//     // NSView *rootView = [[NSView alloc] initWithFrame:window.frame];
//     // rootView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
//     // auto it = windowWidgetRootViews.begin();
//     // while(it != windowWidgetRootViews.end()){
//     //     CocoaItem *item = (CocoaItem *)*it;
//     //     NSViewController *view = (NSViewController *)item->getBinding();
//     //     [rootView addSubview:view.view];
//     //     ++it;
//     // };
//     // [window setContentView:rootView];
// };

void CocoaAppWindow::initialDisplay(){
    [windowController.window center];
    // NSView *rootView = [[NSView alloc] initWithFrame:window.frame];
    // rootView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    // [window setContentView:rootView];
    // // [window layoutIfNeeded];
    // // if(menu){
    // //     NSMenu * windowMenu = (NSMenu *)menu->getNativeBinding();
    // //     [window setMenu:windowMenu];
    // //     [NSApp setMainMenu:windowMenu];
    // // };
    NSLog(@"Display Window");
    [windowController showWindow:windowDelegate];
    [windowController.window makeKeyAndOrderFront:nil];
    NSLog(@"IS Visible :%d",[windowController.window isVisible]);
};

void CocoaAppWindow::close(){
    if([windowController.window isVisible] == YES)
        [windowController.window close];
};

 __strong NSWindow *CocoaAppWindow::getWindow(){
     return windowController.window;
 };

// CocoaAppWindow::~CocoaAppWindow(){
//     close();
// };

};

@implementation OmegaWTKNativeCocoaAppWindowDelegate

-(void)emitIfPossible:(OmegaWTK::Native::NativeEventPtr)event{
    if(self.cppBinding->hasEventEmitter()){
        self.cppBinding->getEmitter()->emit(event);
    };
};
-(void)windowWillClose:(NSNotification *)notification {
    OmegaWTK::Native::NativeEventPtr event = new OmegaWTK::Native::NativeEvent(OmegaWTK::Native::NativeEvent::WindowWillClose,nullptr);
    [self emitIfPossible:event];
    
};
-(void)windowDidResize:(NSNotification *)notification 
{
    // CGFloat scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    // NSLog(@"Window FRAME: {x:%f,y:%f,w:%f,h:%f}",_cppBinding->window.contentView.frame.origin.x,_cppBinding->window.contentView.frame.origin.y,_cppBinding->window.contentView.frame.size.width,_cppBinding->window.contentView.frame.size.height);
    // OmegaWTK::Native::WindowWillResize *params = new OmegaWTK::Native::WindowWillResize(OmegaWTK::Rect(_cppBinding->window.contentView.frame.origin.x,_cppBinding->window.contentView.frame.origin.y,_cppBinding->window.contentView.frame.size.width,_cppBinding->window.contentView.frame.size.height));
    // OmegaWTK::Native::NativeEventPtr event = new OmegaWTK::Native::NativeEvent(OmegaWTK::Native::NativeEvent::WindowWillResize,params);
    // [self emitIfPossible:event];
};

@end

@implementation OmegaWTKNativeCocoaAppWindowController
- (instancetype)initWithRect:(NSRect) rect delegate:(id<NSWindowDelegate>) delegate
{
    self = [super initWithWindow:[[NSWindow alloc] initWithContentRect:rect styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable backing:NSBackingStoreBuffered defer:NO]];
    if (self) {
        NSWindow *window = self.window;
        window.delegate = delegate;
    }
    return self;
}

@end
