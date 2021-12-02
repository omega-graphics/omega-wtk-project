#include <OmegaWTKM.h>

#include <android_native_app_glue.h>

void android_app_on_create(android_app *app){
    auto inst = new OmegaWTK::Mobile::App((void *)app);
    int rc = omegaWTKMain(inst);
    delete inst;
}