#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeApp.h"
#include "AppWindow.h"

#ifndef OMEGAWTK_UI_APP_H
#define OMEGAWTK_UI_APP_H


namespace OmegaWTK {


class OMEGAWTK_EXPORT AppInst : public Native::NativeThemeObserver {
    Native::NAP ptr;
    static AppInst *instance;
public:
    OMEGACOMMON_CLASS("OmegaWTK.AppInst")
    void onThemeSet(Native::ThemeDesc &desc) override;
    static  AppInst * inst();
    UniqueHandle<AppWindowManager> windowManager;
    explicit AppInst(void *data);
    static int start();
    static void terminate();

   
// #ifdef TARGET_WIN32
//     AppInst(void * windows_inst);
// #endif
    Native::NAP & getNAP(){ return ptr;};
    ~AppInst() override;
};


    
};

// #ifdef TARGET_WIN32 
// #ifdef WINDOWS_PRIVATE

// OMEGAWTK_EXPORT RECT get_hwnd_item_coords(void * hwnd);

// #endif
// #endif

#endif
