#include <OmegaWTK.h>

using namespace OmegaWTK;

int omegaWTKMain(OmegaWTK::AppInst *app){

    auto window = make<AppWindow>(Core::Rect {Core::Position {0,0},500,500});

    // auto treeHost = WidgetTreeHost::Create();


    // treeHost->attachToWindow(window);

    app->windowManager->setRootWindow(window);
    app->windowManager->displayRootWindow();
     
    return 0;
};
