#import "omegaWTKM/Native/NativeWindowNavigator.h"

#import <UIKit/UIKit.h>

namespace OmegaWTK::Mobile::Native {

    class IOSWindow : public NativeWindow {
        __strong UIWindow *window;
    public:
        explicit IOSWindow(UIWindow *window):window(window){
            
        }
        void setRootView(NativeItemPtr view) override {

        }
    };

    class IOSWindowNavigator : public NativeWindowNavigator {
        __strong UIWindowScene *scene;
    public:
        explicit IOSWindowNavigator(UIWindowScene * scene):scene(scene){ 
            
        };

       NativeWindowPtr newWindow() override {
           return (NativeWindowPtr)new IOSWindow([[UIWindow alloc] initWithWindowScene:scene]);
       }

        void setKeyWindow(unsigned int idx) override {
            [scene.windows[idx] makeKeyWindow];
        }
    };

    NativeWindowNavigator *CreateIOSWindowNavigator(UIWindowScene * scene){
        return new IOSWindowNavigator(scene);
    };
}