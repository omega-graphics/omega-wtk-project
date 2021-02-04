#include "omegaWTK/UI/App.h"
#include "omegaWTK/Native/NativeApp.h"


namespace OmegaWTK {

AppInst * AppInst::instance;

AppInst::AppInst():rootWidget(nullptr),ptr(new Native::NativeApp()){
    instance = this;
};

void internal_set_root(View *view,Native::NAP ptr){
    ptr->setNativeItemPtr(view->native);
};

void AppInst::setRoot(Widget *widget){
    rootWidget = widget;
    internal_set_root(widget->rootView,ptr);
};

AppInst::~AppInst(){
    instance = nullptr;
};

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void * create_hwnd_factory(void * hinst){
    return __create_hwnd_factory(hinst);
};

void free_hwnd_factory(void *factory){
    return __free_hwnd_factory(factory);
};


#endif
#endif
