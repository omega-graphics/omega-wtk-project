#import <Cocoa/Cocoa.h>
#import "CocoaApp.h"

namespace OmegaWTK::Native::Cocoa {

int nsapp_init(int argc,const char *argv[]){
    return NSApplicationMain(argc,argv);
};

};
