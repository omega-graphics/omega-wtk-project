#include "omegaWTK/Core/Core.h"

#ifdef TARGET_WIN32
#include <Windows.h>
#endif

#ifdef TARGET_MACOS
#import <AppKit/AppKit.h>
#endif

#ifndef OMEGAWTK_WIDGETS_APP_H
#define OMEGAWTK_WIDGETS_APP_H




#if defined(TARGET_WIN32)
#define OMEGAWTK_APP_ENTRY_BEGIN int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
#define OMEGAWTK_INIT_APP(app) 1
#define OMEGAWTK_APP_ENTRY_END }
#elif defined(TARGET_MACOS)
#define OMEGAWTK_APP_ENTRY_BEGIN
#define OMEGAWTK_INIT_APP(app)
#define OMEGAWTK_APP_ENTRY_END
#endif

namespace OmegaWTK {

    namespace Native {
    class NativeAppMenuItem;
    class NativeAppMenu;
    class NativeApp;
    
    typedef NativeAppMenuItem *NAMI;
    typedef NativeAppMenu *NAM;
    typedef NativeApp *NativeAppPtr;
    };
    
    
    class AppMenuItem implements(Clickable) {
        Core::Vector<AppMenuItem> sub_menu;
        Native::NAMI native;
    public:
        void onHover(Native::NativeEventCallback callback);
        void onClick(Native::NativeEventCallback callback);
        AppMenuItem();
        ~AppMenuItem();
    };
    class AppMenu {
        Core::Vector<AppMenuItem> menu;
        Native::NAM native;
    public:
        AppMenu();
        ~AppMenu();
    };
    class App {
        AppMenu main_menu;
        
    };
};

#endif
