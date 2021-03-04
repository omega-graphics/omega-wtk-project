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
    window = windowController.window;
};

NativeEventEmitter * CocoaAppWindow::getEmitter() {
    return eventEmitter;
};

void CocoaAppWindow::disable(){
    if([window isVisible] == YES){
        [window setIsVisible:NO];
    };
};

void CocoaAppWindow::enable(){
    if([window isVisible] == NO){
        [window setIsVisible:YES];
    };
};

void CocoaAppWindow::attachWidgets(){
    NSView *rootView = [[NSView alloc] initWithFrame:NSZeroRect];
    rootView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    auto it = windowWidgetRootViews.begin();
    while(it != windowWidgetRootViews.end()){
        CocoaItem *item = (CocoaItem *)*it;
        NSView *view = (NSView *)item->getBinding();
        [rootView addSubview:view];
        ++it;
    };
    [window setContentView:rootView];
};

void CocoaAppWindow::initialDisplay(){
    [window center];
    [window layoutIfNeeded];
    if(menu){
        NSMenu * windowMenu = (NSMenu *)menu->getNativeBinding();
        [window setMenu:windowMenu];
        [NSApp setMainMenu:windowMenu];
    };
    
    [windowController showWindow:NSApp.delegate];
};

};

@implementation OmegaWTKNativeCocoaAppWindowDelegate
-(void) emitIfPossible:(OmegaWTK::Native::NativeEventPtr)event{
    if(self.cppBinding->hasEventEmitter()){
        self.cppBinding->getEmitter()->emit(event);
    };
};
-(void)windowWillClose:(NSNotification *)notification {
    OmegaWTK::Native::NativeEventPtr event = new OmegaWTK::Native::NativeEvent(OmegaWTK::Native::NativeEvent::WindowWillClose,nullptr);
    [self emitIfPossible:event];
};
//-(NSSize)windowWillResize:(NSWindow *)sender toSize:(NSSize)frameSize;
//-(void)windowDidEndLiveResize:(NSNotification *)notification;
//-(void)windowDidResize:(NSNotification *)notification;

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
