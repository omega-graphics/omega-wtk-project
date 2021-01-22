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
            HWND currentParent;
            public:
            static HWNDFactory * appFactoryInst;
            void setRootWindowAndHINST(HWND root,HINSTANCE hinst);
            /// NOTE: Only call if in Application and/or Composition Libs!
            void setCurrentParent(HWND parent);
            HWND makeWindow(LPCSTR class_name,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params);
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