#include "omegaWTK/UI/App.h"
//#include "omegaWTK/Native/NativeApp.h"


namespace OmegaWTK {

AppInst * AppInst::instance = nullptr;

AppInst::AppInst(Widget *root):root(root){
    instance = this;
};

AppInst::~AppInst(){
    instance = nullptr;
};

};
