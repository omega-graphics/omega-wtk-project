#include <OmegaWTK.h>


int main(int argc,const char * argv[]){
    auto * app = new OmegaWTK::AppInst(nullptr);
    omegaWTKMain(app);
    delete app;
    return 0;
};