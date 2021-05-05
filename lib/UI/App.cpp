#include "omegaWTK/UI/App.h"
#include "omegaWTK/Native/NativeApp.h"
#include "omegaWTK/UI/Menu.h"
#include "omegaWTK/Composition/FontEngine.h"


namespace OmegaWTK {



AppInst * AppInst::instance;

AppInst::AppInst():ptr(Native::make_native_app()),windowManager(std::make_unique<AppWindowManager>()){
    instance = this;
    Composition::FontEngine::Create();
    FS::Path assets_path = "./assets.omxa";
    if(assets_path.exists())
        loadAssetFile("./assets.omxa");
};

void AppInst::terminate() {
    windowManager->closeAllWindows();
    ptr->terminate();
};

// void AppInst::updateRootWidget()
// {
//     rootWidget->compositor->prepareUpdate();
// };

AppInst::~AppInst(){
    instance = nullptr;
    Composition::FontEngine::Destroy();
};

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void * create_hwnd_factory(void * hinst){
    return __create_hwnd_factory(hinst);
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
