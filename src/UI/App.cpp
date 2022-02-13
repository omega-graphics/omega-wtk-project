#include "omegaWTK/UI/App.h"
#include "omegaWTK/UI/Menu.h"
#include "omegaWTK/Composition/FontEngine.h"
#include "omegaWTK/Native/NativeTheme.h"

#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/UI/AppWindow.h"

namespace OmegaWTK {



AppInst * AppInst::instance;

AppInst *AppInst::inst() {
    return instance;
}

AppInst::AppInst(void *data):ptr(Native::make_native_app(data)),windowManager(std::make_unique<AppWindowManager>()){
    instance = this;
    gte = OmegaGTE::InitWithDefaultDevice();
    Composition::InitializeEngine();
    OMEGAWTK_DEBUG("Application Startup")
    Composition::FontEngine::Create();
    OmegaCommon::FS::Path assets_path("./assets.omxa");
    if(assets_path.exists())
        loadAssetFile("./assets.omxa");
};

int AppInst::start(){
    return instance->ptr->runEventLoop();
};

void AppInst::terminate() {
    instance->windowManager->closeAllWindows();
    instance->ptr->terminate();
};

void AppInst::onThemeSet(Native::ThemeDesc &desc) {

}

AppInst::~AppInst(){
    instance = nullptr;
    Composition::FontEngine::Destroy();
    Composition::CleanupEngine();
    OmegaGTE::Close(gte);
    OMEGAWTK_DEBUG("Application Shutdown")
};

};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

// void * create_hwnd_factory(void * hinst){
//     return __create_hwnd_factory(hinst);
// };

// void *hwnd_factory_get_all_hwnds(void *hwnd_factory){
//     return __hwnd_factory_get_all_hwnds(hwnd_factory);
// };

// void free_hwnd_factory(void *factory){
//     return __free_hwnd_factory(factory);
// };

RECT get_hwnd_item_coords(void * hwnd){
    return __get_hwnd_real_coords((HWND)hwnd);
};


#endif
#endif
