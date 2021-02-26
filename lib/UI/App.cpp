#include "omegaWTK/UI/App.h"
#include "omegaWTK/Native/NativeApp.h"
#include "omegaWTK/UI/Menu.h"


namespace OmegaWTK {



AppInst * AppInst::instance;

AppInst::AppInst():ptr(new Native::NativeApp()){
    instance = this;
};

void internal_add_to_root(View *view,Native::NAP ptr){
    ptr->addNativeItemPtr(view->getLayer()->getTargetNativePtr());
};

void AppInst::addWidgetToRoot(Widget *widget){
    widget->compositor->prepareDraw(widget->rootView->getLayer());
    internal_add_to_root(widget->rootView,ptr);
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

RECT get_hwnd_item_coords(void * hwnd){
    return __get_hwnd_real_coords((HWND)hwnd);
};


#endif
#endif
