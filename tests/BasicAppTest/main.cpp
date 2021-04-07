#include <OmegaWTK.h>

using namespace OmegaWTK;

class MyWindowDelegate : public AppWindowDelegate {
    AppInst *app;
public:
    MyWindowDelegate(AppInst *app):app(app){};
    void windowWillClose(Native::NativeEventPtr event){
        app->terminate();
    };
};


int omegaWTKMain(AppInst *app){

    auto appWindow = make<AppWindow>(OmegaWTK::Rect(0,0,1000,1000),new MyWindowDelegate(app));

    app->windowManager->setRootWindow(appWindow);
    app->windowManager->displayRootWindow();

    return 0;
};