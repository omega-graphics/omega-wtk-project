#include <Windows.h>
#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_WIN_HWNDFACTORY_H
#define OMEGAWTK_NATIVE_WIN_HWNDFACTORY_H

namespace OmegaWTK::Native{
    namespace Win {
        /// Factory for registering and creating HWNDs.
        /// NOTE: Only instiantitate this class once!
        class HWNDFactory {
            HWND rootWindow;
            HINSTANCE hInst;
            Core::Vector<Core::String> wndclassregistry;
            /// Root hwnd children!
            Core::Vector<HWND> all_hwnds;
            static LRESULT WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
            public:
            static HWNDFactory * appFactoryInst;
            auto getAllHwnds(){ return &all_hwnds;};
            void setRootWindowAndHINST(HWND root,HINSTANCE hinst);
            HWND getRootWnd();
            HWND makeWindow(ATOM atom,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,HWND parent,DWORD ext_style = NULL);
            ATOM registerWindow();
            // One HWND Factory Allowed!
            HWNDFactory(HINSTANCE hinst,HWND rootHwnd);
            ~HWNDFactory();
        };
    };
};

#endif