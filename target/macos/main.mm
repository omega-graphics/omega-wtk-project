#include <OmegaWTK.h>

int main(int argc,const char * argv[]){
    auto * app = new OmegaWTK::AppInst(nullptr);
    auto rc = omegaWTKMain(app);
    delete app;
    return rc;
};