#include <OmegaWTK.h>

using namespace OmegaWTK;

int omegaWTKMain(OmegaWTK::AppInst *app){

    auto window = make<AppWindow>(Core::Rect {1000,1000});

    app->windowManager->setRootWindow(window);
    app->windowManager->displayRootWindow();
     
    return 0;
};
