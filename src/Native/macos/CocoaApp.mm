#import <Cocoa/Cocoa.h>
#include "omegaWTK/Native/NativeApp.h"

#import <Metal/Metal.h>

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
    void terminate() override{
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0];
    };
    int runEventLoop() override {
        [NSApp run];
        return 0;
    };
    ~CocoaApp() override {
        MTLCaptureManager *manager = [MTLCaptureManager sharedCaptureManager];
        [manager stopCapture];
    };
};

};

namespace OmegaWTK::Native {
    NAP make_native_app(void *data){
        return (NAP)new Cocoa::CocoaApp();
    }
}
