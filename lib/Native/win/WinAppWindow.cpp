#include "WinAppWindow.h"
#include "HWNDFactory.h"

namespace OmegaWTK::Native::Win {
    WinAppWindow::WinAppWindow(Core::Rect & rect):HWNDItem(rect){
        atom = HWNDFactory::appFactoryInst->registerAppWindow();
        hwnd = HWNDFactory::appFactoryInst->makeAppWindow(atom,"",rect,WS_OVERLAPPEDWINDOW,WS_EX_OVERLAPPEDWINDOW | WS_EX_LAYERED,this);

        currentDpi = GetDpiForWindow(hwnd);
        isTracking = false;
        hovered = false;
    };

    BOOL WinAppWindow::ProcessWndMsgImpl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lr){
        *lr = 0;
        switch (uMsg) {
        // case WM_CREATE : {
        //     break;
        // };
        // case WM_SIZE : {
        //     break;
        // };
        // case WM_SIZING : {
        //     break;
        // };
        // case WM_MOVE : {
        //     break;
        // };
        default:
            return FALSE;
        }
        return TRUE;
    };
};

