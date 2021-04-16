#include <OmegaWTK.h>

using namespace OmegaWTK;

int omegaWTKMain(OmegaWTK::AppInst *app) {

    auto appWindow = make<AppWindow>(OmegaWTK::Rect(0,0,1000,1000),nullptr);

    

    app->windowManager->setRootWindow(appWindow);
    app->windowManager->displayRootWindow();

    return 0;
}