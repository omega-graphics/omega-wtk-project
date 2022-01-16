#import <UIKit/UIKit.h>

#include "omegaWTKM/Native/NativeMobileApp.h"

namespace OmegaWTK::Mobile::Native {
    class UIKitApp;
}

@interface OmegaWTKUIKitAppDelegate : NSObject
-(instancetype)initWithPtr:(OmegaWTK::Mobile::Native::UIKitApp *)ptr;
@end


namespace OmegaWTK::Mobile::Native {

    NativeWindowNavigator *CreateIOSWindowNavigator(UIWindowScene * scene);


    class UIKitApp : public NativeApp {
        __strong UIApplication *app;
        public:

        SharedHandle<NativeWindowNavigator> createNavigator() override {
            [app requestSceneSessionActivation:nil userActivity:app.userActivity options:nil errorHandler:^(NSError *error){

            }];
           
            UIScene *scene = app.openSessions.allObjects.lastObject.scene;
            return (SharedHandle<NativeWindowNavigator>)CreateIOSWindowNavigator((UIWindowScene *)scene);
        }

        void setDefaultNavigator(SharedHandle<NativeWindowNavigator> & navigator) override {
            
        }

        int run() override {
            return (int)app.applicationState;
        }

        void terminate() override{
            [app performSelector:@selector(terminateWithSuccess:) withObject:nil afterDelay:0];
        };
    };

    NAP make_native_app(){
        return (NAP)new UIKitApp();
    };
    
};