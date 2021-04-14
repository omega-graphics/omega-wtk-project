#include "omegaWTK/Core/Core.h"
// #include "omegaWTK/Native/NativeApp.h"
#include "AppWindow.h"

#ifndef OMEGAWTK_UI_APP_H
#define OMEGAWTK_UI_APP_H


namespace OmegaWTK {

    namespace Native {
        class NativeApp;
    };

class OMEGAWTK_EXPORT AppInst {
    Native::NativeApp * ptr;
public:
    UniqueHandle<AppWindowManager> windowManager;
    static AppInst *instance;
    AppInst();
    void terminate();
// #ifdef TARGET_WIN32
//     AppInst(void * windows_inst);
// #endif
    Native::NativeApp * getNAP(){ return ptr;};
    ~AppInst();
};


    
};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

OMEGAWTK_EXPORT void * create_hwnd_factory(void * hinst);
OMEGAWTK_EXPORT void * hwnd_factory_get_all_hwnds(void *hwnd_factory);
OMEGAWTK_EXPORT void free_hwnd_factory(void *factory);
OMEGAWTK_EXPORT RECT get_hwnd_item_coords(void * hwnd);

#endif
#endif

#endif
