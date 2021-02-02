#include "omegaWTK/Core/Core.h"
#include "Widget.h"

#ifndef OMEGAWTK_UI_APP_H
#define OMEGAWTK_UI_APP_H


#if defined(TARGET_WIN32)
#define OMEGAWTK_APP_ENTRY_BEGIN int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
#define OMEGAWTK_INIT_APP(app) 1
#define OMEGAWTK_APP_ENTRY_END }
#elif defined(TARGET_MACOS)
#define OMEGAWTK_APP_ENTRY_BEGIN int main(int argc,char *argv[]) {
#define OMEGAWTK_INIT_APP(app)
#define OMEGAWTK_APP_ENTRY_END }
#endif

namespace OmegaWTK {

class OMEGAWTK_EXPORT AppInst {
    Widget *root;
public:
    static AppInst *instance;
    AppInst(Widget *root);
    ~AppInst();
};
    
};

#endif
