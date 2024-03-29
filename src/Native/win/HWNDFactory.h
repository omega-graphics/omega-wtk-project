#include <Windows.h>
#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_WIN_HWNDFACTORY_H
#define OMEGAWTK_NATIVE_WIN_HWNDFACTORY_H

namespace OmegaWTK::Native{
    namespace Win {
        void updateAllHWNDPos(UINT root_wnd_height,OmegaCommon::Vector<HWND> * hwnds_to_update);
        /// Factory for registering and creating HWNDs.
        /// NOTE: Only instiantitate this class once!
        class OMEGAWTK_EXPORT HWNDFactory {
            OmegaCommon::Vector<OmegaCommon::String> wndclassregistry;
            /// Root hwnd children!
            OmegaCommon::Vector<HWND> all_hwnds;
            static LRESULT WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
            static LRESULT WndProc2(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
            public:
            HINSTANCE hInst;
            static HWNDFactory * appFactoryInst;
            auto getAllHwnds(){ return &all_hwnds;};
            void setRootWindowAndHINST(HWND root,HINSTANCE hinst);
            // HWND getRootWnd();
            HWND makeWindow(ATOM atom,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,HWND parent,DWORD ext_style = NULL);
            ATOM registerWindow();
            ATOM registerAppWindow();
            HWND makeAppWindow(ATOM atom,LPCSTR name,Core::Rect & rect,DWORD base_style,DWORD ext_style,LPVOID custom_params);
            // One HWND Factory Allowed!
            HWNDFactory(HINSTANCE hinst);
            ~HWNDFactory();
        };
        void *getHWNDUserData(HWND hwnd);
    };
};

#endif