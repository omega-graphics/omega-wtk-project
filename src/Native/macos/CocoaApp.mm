#import <Cocoa/Cocoa.h>
#import "CocoaApp.h"
#include "omegaWTK/Native/NativeApp.h"

namespace OmegaWTK::Native::Cocoa {

class CocoaApp : public NativeApp {
public:
    void terminate(){
        [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0];
    };
};

NAP make_cocoa_app(){
    return new CocoaApp();
};

};
