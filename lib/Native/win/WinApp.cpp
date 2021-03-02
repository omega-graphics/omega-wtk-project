#include "WinApp.h"
#include <Windows.h>

namespace OmegaWTK::Native::Win {
    class WinApp : public NativeApp {
    public:
        void terminate(){
            PostQuitMessage(0);
        };
    };
    
    NAP make_win_app(){
        return new WinApp();
    };
};