#import <UIKit/UIKit.h>

#include "omegaWTKM/Native/NativeMobileApp.h"


namespace OmegaWTK::Mobile::Native {

    class OMEGAWTK_EXPORT UIKitApp : public NativeApp {
        __strong UIApplication *app;
        public:

        void terminate(){
            [app performSelector:@selector(terminateWithSuccess:) withObject:nil afterDelay:0];
        };
    };

    NAP make_native_app(){
        return new UIKitApp();
    };
    
};