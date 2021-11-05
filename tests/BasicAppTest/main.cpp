#include <OmegaWTK.h>

int omegaWTKMain(OmegaWTK::AppInst *app){
    auto widgetTreeHost = OmegaWTK::WidgetTreeHost::Create();

    return app->start();
};