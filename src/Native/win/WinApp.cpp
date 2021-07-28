#include "WinApp.h"
#include <Windows.h>

namespace OmegaWTK::Native::Win {
    class WinApp : public NativeApp {
    public:
        void terminate(){
            PostQuitMessage(0);
        };
        void runEventLoop(){
            // HACCEL hAccelTable = LoadAccelerators(hInstance,MAKEINTRESOURCE(IDC_@APPNAME@));

            MSG msg = {};
            while (GetMessage(&msg,nullptr,0,0))
            {
                // if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
                // {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                // }
            }
            return msg.wParam;
        };
    };
    
    NAP make_win_app(){
        return new WinApp();
    };
};