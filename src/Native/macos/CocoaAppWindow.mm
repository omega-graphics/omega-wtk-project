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
     CGFloat scaleFactor = [NSScreen mainScreen].backingScaleFactor;
     NSLog(@"Window FRAME: {x:%f,y:%f,w:%f,h:%f}",self.window.contentView.frame.origin.x,self.window.contentView.frame.origin.y,self.window.contentView.frame.size.width,self.window.contentView.frame.size.height);
     auto *params = new OmegaWTK::Native::WindowWillResize(
             OmegaWTK::Core::Rect
                     {(float)self.window.contentView.frame.origin.x,
                      (float)self.window.contentView.frame.origin.y,
                      (float)self.window.contentView.frame.size.width,
                      (float)self.window.contentView.frame.size.height}
     );
     auto event = new OmegaWTK::Native::NativeEvent(OmegaWTK::Native::NativeEvent::WindowWillResize,params);
     [self emitIfPossible:event];
};

@end

@implementation OmegaWTKNativeCocoaAppWindowController
- (instancetype)initWithRect:(NSRect) rect delegate:(id<NSWindowDelegate>) delegate
{
    self = [super initWithWindow:[[NSWindow alloc] initWithContentRect:rect styleMask:NSWindowStyleMaskBorderless | NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable backing:NSBackingStoreBuffered defer:NO]];
    if (self) {
        NSWindow *window = self.window;
        window.delegate = delegate;
    }
    return self;
}

@end

namespace OmegaWTK::Native {
    NWH make_native_window(Core::Rect & rect,NativeEventEmitter *emitter){
        return new Cocoa::CocoaAppWindow(rect,emitter);
    };
}
