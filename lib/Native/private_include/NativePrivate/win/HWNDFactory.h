#include <Windows.h>
#include "omegaWTK/Core/Core.h"
#include <functional>

#ifndef OMEGAWTK_NATIVE_WIN_HWNDFACTORY_H
#define OMEGAWTK_NATIVE_WIN_HWNDFACTORY_H

namespace OmegaWTK::Native{
    namespace Win {
        /// Factory for registering and creating HWNDs.
        /// NOTE: Only instiantitate this class once!
        class HWNDFactory {
            HWND rootWindow;
            HINSTANCE hInst;
            std::vector<std::string> wndclassregistry;
            static CALLBACK LRESULT WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
            public:
            static HWNDFactory * appFactoryInst;
            void setRootWindowAndHINST(HWND root,HINSTANCE hinst);
            HWND makeWindow(ATOM atom,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,DWORD ext_style = NULL);
            ATOM registerWindow();
            // One HWND Factory Allowed!
            HWNDFactory(){
                appFactoryInst = this;
            };
            ~HWNDFactory(){};
        };
    };
};

#endif