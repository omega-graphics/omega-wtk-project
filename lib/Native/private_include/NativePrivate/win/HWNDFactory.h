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
            std::vector<std::string> wndclassregistry;
            public:
            static HWNDFactory * appFactoryInst;
            void setRootWindowAndHINST(HWND root,HINSTANCE hinst);
            HWND makeWindow(LPCSTR class_name,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,DWORD ext_style = NULL);
            ATOM registerWindow(LPCSTR class_name,WNDPROC proc_ptr);
            // One HWND Factory Allowed!
            HWNDFactory(){
                appFactoryInst = this;
            };
            ~HWNDFactory(){};
        };
    };
};

#endif