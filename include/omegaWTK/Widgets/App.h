#include "omegaWTK/Core/Core.h"

#ifdef TARGET_WIN32
#include <Windows.h>
#endif

#ifdef TARGET_MACOS
#endif

#ifndef OMEGAWTK_WIDGETS_APP_H
#define OMEGAWTK_WIDGETS_APP_H




#if defined(TARGET_WIN32)
#define OMEGAWTK_APP_ENTRY_BEGIN int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
#define OMEGAWTK_INIT_APP(app) 1
#define OMEGAWTK_APP_ENTRY_END }
#elif defined(TARGET_MACOS)
#define OMEGAWTK_APP_ENTRY_BEGIN
#define OMEGAWTK_INIT_APP(app)
#define OMEGAWTK_APP_ENTRY_END
#endif

namespace OmegaWTK {
    class App {
        
    };
};

#endif