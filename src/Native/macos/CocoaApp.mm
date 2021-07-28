#import <Cocoa/Cocoa.h>
#import "CocoaApp.h"
#include "omegaWTK/Native/NativeApp.h"



namespace OmegaWTK::Native::Cocoa {

class CocoaApp : public NativeApp {
    void *app;
public:
    CocoaApp(){
        app = (__bridge void *)[NSApplication sharedApplication];

        [[NSNotificationCenter defaultCenter]
		    postNotificationName:NSApplicationWillFinishLaunchingNotification
		    object:NSApp];
	    [[NSNotificationCenter defaultCenter]
		    postNotificationName:NSApplicationDidFinishLaunchingNotification
		    object:NSApp];

    };
    void terminate(){
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0];
    };
    int runEventLoop() {
        [NSApp run];
        return 0;
    };
    ~CocoaApp(){
        
    };
};

NAP make_cocoa_app(){
    return new CocoaApp();
};

};
