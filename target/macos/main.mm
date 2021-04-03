#import <Cocoa/Cocoa.h>
#import <mach-o/dyld.h>
#include <dlfcn.h>

int main(int argc,const char * argv[]){
    NSLog(@"%@",[[NSBundle mainBundle] sharedFrameworksPath]);
    return NSApplicationMain(argc,argv);
};