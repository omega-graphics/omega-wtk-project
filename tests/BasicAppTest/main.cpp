
#include <OmegaWTK.h>

int omegaWTKMain(OmegaWTK::AppInst *app){

    OmegaWTK::AppWindow window(OmegaWTK::Core::Rect {{0,0},500,500});

    auto menu = make<OmegaWTK::Menu>(OmegaWTK::Menu("MyMenu",{
        OmegaWTK::CategoricalMenu("BasicAppTest",{
            OmegaWTK::ButtonMenuItem("Hello World!")
        })
    }));

    window.setMenu(menu);
//    window.setTitle("BasicAppTest");


    auto & windowManager = app->windowManager;

    windowManager->setRootWindow(&window);
    windowManager->displayRootWindow();

    return OmegaWTK::AppInst::start();
};