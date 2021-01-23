#import "AppDelegate.h"

@interface OmegaWTKAppWindowController () <NSWindowDelegate>
@property (nonatomic) NSWindowController *r_self;
@end

@implementation OmegaWTKAppWindowController
- (instancetype)init {
    if(self = [super initWithWindow:[[NSWindow alloc]initWithContentRect:NSZeroRect styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled) backing:NSBackingStoreBuffered defer:NO]]){
        _r_self = self;

        NSWindow *window = self.window;
        window.title = NSProcessInfo.processInfo.processName;
        window.delegate = self;
        [window layoutIfNeeded];
        [window center];
        window.frameAutosaveName = @"Main";
    };
    return self;
}
-(void)windowWillClose:(NSNotification *)notification {
    _r_self = nil;
    [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0];
}
@end


@interface AppDelegate () <NSApplicationDelegate,NSWindowDelegate,NSMenuDelegate>
@property (nonatomic) NSWindow *window;
@end

@implementation AppDelegate
-(void)applicationWillFinishLaunching:(NSNotification *)notification {

}
-(void)applicationDidFinishLaunching:(NSNotification *)notification {
    OmegaWTKAppWindowController *windowController = [[OmegaWTKAppWindowController alloc] init];
    [windowController showWindow:self];
}
-(void)applicationWillTerminate:(NSNotification *)notification {

}
@end