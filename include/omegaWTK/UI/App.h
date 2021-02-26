#include "omegaWTK/Core/Core.h"
// #include "omegaWTK/Native/NativeApp.h"
#include "Widget.h"

#ifndef OMEGAWTK_UI_APP_H
#define OMEGAWTK_UI_APP_H


// #if defined(TARGET_WIN32)
// #define OMEGAWTK_APP_ENTRY_BEGIN int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
// #define OMEGAWTK_INIT_APP(app) 1
// #define OMEGAWTK_APP_ENTRY_END }
// #elif defined(TARGET_MACOS)
// #define OMEGAWTK_APP_ENTRY_BEGIN int main(int argc,char *argv[]) {
// #define OMEGAWTK_INIT_APP(app)
// #define OMEGAWTK_APP_ENTRY_END return OmegaWTK::Native::cocoa_app_init(argc,argv);}
// #endif

namespace OmegaWTK {

    namespace Native {
        class NativeApp;
    };

class Menu;

class OMEGAWTK_EXPORT AppInst {
    Native::NativeApp * ptr;
public:
    static AppInst *instance;
    Menu *menu = nullptr;
    AppInst();
#ifdef TARGET_WIN32
    AppInst(void * windows_inst);
#endif
    void addWidgetToRoot(Widget *widget);
    Native::NativeApp * getNAP(){ return ptr;};
    ~AppInst();
};


    
};

#ifdef TARGET_WIN32 
#ifdef WINDOWS_PRIVATE

void * create_hwnd_factory(void * hinst,void *hwndroot);
void * hwnd_factory_get_all_hwnds(void *hwnd_factory);
void free_hwnd_factory(void *factory);
RECT get_hwnd_item_coords(void * hwnd);

#endif
#endif

#endif
