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

// void AppInst::updateRootWidget()
// {
//     rootWidget->compositor->prepareUpdate();
// };

AppInst::~AppInst(){
    instance = nullptr;
};

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void * create_hwnd_factory(void * hinst,void *hwndroot){
    return __create_hwnd_factory(hinst,hwndroot);
};

void *hwnd_factory_get_all_hwnds(void *hwnd_factory){
    return __hwnd_factory_get_all_hwnds(hwnd_factory);
};

void free_hwnd_factory(void *factory){
    return __free_hwnd_factory(factory);
};


#endif
#endif
