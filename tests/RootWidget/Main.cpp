#include <OmegaWTK.h>

using namespace OmegaWTK;

int omegaWTKMain(OmegaWTK::AppInst *app){

    auto window = make<AppWindow>(Core::Rect {500,500});

    app->windowManager->setRootWindow(window);
    app->windowManager->displayRootWindow();
     
    return 0;
};
