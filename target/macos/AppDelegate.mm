#import "AppDelegate.h"
#include <OmegaWTK.h>
#import "omegaWTK/Main.h"
#include <vector>


// @implementation OmegaWTKAppWindowController
// - (instancetype)init {
//     if(self = [super initWithWindow:[[NSWindow alloc]initWithContentRect:NSZeroRect styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled) backing:NSBackingStoreBuffered defer:NO]]){
//         _r_self = self;

//         NSWindow *window = self.window;
//         window.title = NSProcessInfo.processInfo.processName;
//         window.delegate = self;
//         [window layoutIfNeeded];
//         [window center];
//         window.frameAutosaveName = @"Main";
        
//         OmegaWTK::AppInst inst;
//         omegaWTKMain(&inst);
        
//         std::vector<void *> bindings = inst.getNAP()->getNativeItemsWithBindings();
//         OmegaWTKRootView *global = [[OmegaWTKRootView alloc] init];
//         for(void * view : bindings){

//             [global addSubview:(NSView *)view];
//         };
//         [window setContentView:global];
//         if(inst.menu != nullptr) {
//             NSMenu *menu = (NSMenu *)inst.menu->getNativeMenu()->getNativeBinding();
//             NSLog(@"Mounting Menu %@",menu);
//             NSMenuItem *applicationBaseMenuI = [[NSMenuItem alloc] initWithTitle:@"@APPNAME@" action:nil keyEquivalent:@""];
//               NSMenu *applicationBaseMenu = [[NSMenu alloc] init];
//               [applicationBaseMenu addItemWithTitle:@"About @APPNAME@" action:nil keyEquivalent:@""];
//               [applicationBaseMenu addItemWithTitle:@"Quit @APPNAME@" action:@selector(closeApplication) keyEquivalent:@""];
//             [applicationBaseMenuI setSubmenu:applicationBaseMenu];

//             [menu insertItem:applicationBaseMenuI atIndex:0];
//             [NSApp setMainMenu:menu];
//         }
//     };
//     return self;
// }
// -(void)closeApplication {
//     [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0];
// };
// -(void)windowWillClose:(NSNotification *)notification {
//     _r_self = nil;
//     [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0];
// }
// @end


@interface AppDelegate () <NSApplicationDelegate>
// @property (nonatomic) NSWindow *rootWindow;
@end

@implementation AppDelegate {
    OmegaWTK::AppInst *appInst;
}
// -(void)applicationWillFinishLaunching:(NSNotification *)notification {

// }
-(void)applicationDidFinishLaunching:(NSNotification *)notification {
   appInst = new OmegaWTK::AppInst();
   omegaWTKMain(appInst);

}
-(void)applicationWillTerminate:(NSNotification *)notification {
    delete appInst;
}
@end
