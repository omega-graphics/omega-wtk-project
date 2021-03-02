#include "NativePrivate/win/HWNDItem.h"
#include "omegaWTK/Native/NativeWindow.h"

#ifndef OMEGAWTK_NATIVE_WIN_WINAPPWINDOW_H
#define OMEGAWTK_NATIVE_WIN_WINAPPWINDOW_H

namespace OmegaWTK::Native::Win {
    class WinAppWindow : public NativeWindow, public HWNDItem {
        public:
        BOOL ProcessWndMsgImpl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lr) override;
        WinAppWindow(Core::Rect & rect);
    };
};

#endif