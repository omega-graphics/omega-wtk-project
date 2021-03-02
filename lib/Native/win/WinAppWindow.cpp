#include "WinAppWindow.h"
#include "HWNDFactory.h"
#include "WinMenu.h"

namespace OmegaWTK::Native::Win {
    WinAppWindow::WinAppWindow(Core::Rect & rect,NativeEventEmitter *emitter):HWNDItem(rect),isReady(false){
        set_native_item_event_emitter(this,emitter);
        // MessageBoxA(HWND_DESKTOP,"Creating WinAppWindow!","NOTE",MB_OK);
        atom = HWNDFactory::appFactoryInst->registerAppWindow();
        if(!atom){
            MessageBoxA(HWND_DESKTOP,"Failed to Register Desktop Window",NULL,MB_OK);
        };
        HWNDFactory::appFactoryInst->makeAppWindow(atom,"",rect,WS_OVERLAPPEDWINDOW,WS_EX_LAYERED,(void *)this);
        if(hwnd == NULL){
            MessageBoxA(HWND_DESKTOP,"Failed to Create Desktop Window",NULL,MB_OK);
        }
        // else
            // MessageBoxA(HWND_DESKTOP,"Registered Win Window!","NOTE",MB_OK);
        currentDpi = GetDpiForWindow(hwnd);
        isTracking = false;
        hovered = false;
    };

    LRESULT WinAppWindow::ProcessWndMsg(UINT u_int,WPARAM wParam,LPARAM lParam){
        LRESULT result = 0;
        // MessageBoxA(HWND_DESKTOP,"WinAppWindow WndProc","NOTE",MB_OK);
        if(u_int == WM_NCDESTROY){
            hwnd = nullptr;
        };
        if(!ProcessWndMsgImpl(hwnd,u_int,wParam,lParam,&result))
            result = DefWindowProcA(hwnd,u_int,wParam,lParam);
        return result;
    };

    BOOL WinAppWindow::ProcessWndMsgImpl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lr){
        *lr = 0;
        // MessageBoxA(HWND_DESKTOP,"WinAppWindow Procedure","NOTE",MB_OK);
        switch (uMsg) {
            case WM_MENUCOMMAND : {
                /// If the Top level Menu Exists!
                if(menu){
                    UINT idx = wParam;
                    HMENU hmenu = (HMENU)lParam;

                    MENUINFO info;
                    info.cbSize = sizeof(info);
                    info.fMask = MIM_MENUDATA;
                    GetMenuInfo(hmenu,&info);
                    void * WinMenu = (void *) info.dwMenuData;
                    select_item(WinMenu,idx);
                    return 0;
                };
            };
            case WM_DESTROY : {
                if(isReady) {
                    emitIfPossible(new NativeEvent(NativeEvent::WindowWillClose,nullptr));
                };
                break;
            };
            case WM_SIZE : {
                if(isReady) {
                    RECT rc = getClientRect();
                    UINT height = rc.bottom - rc.top;
                    updateAllHWNDPos(height,&raw_children);
                };
                break;
            };
            case WM_SIZING : {
                if(isReady) {
                    RECT rc = getClientRect();
                    UINT height = rc.bottom - rc.top;
                    updateAllHWNDPos(height,&raw_children);
                };
                break;
            };
            case WM_PAINT : {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd,&ps);

                // parentLayer->redraw();

                EndPaint(hwnd,&ps);
                break;
            };
            case WM_DPICHANGED : {
                UINT newDpi = HIWORD(wParam);

                RECT* const prcNewWindow = (RECT*)lParam;
                SetWindowPos(hwnd,
                    NULL,
                    prcNewWindow ->left,
                    prcNewWindow ->top,
                    prcNewWindow->right - prcNewWindow->left,
                    prcNewWindow->bottom - prcNewWindow->top,
                    SWP_NOZORDER | SWP_NOACTIVATE);
                break;
            }
            default:
                return FALSE;
                break;
            }
        return TRUE;
    };

    void WinAppWindow::attachWidgets(){
        auto it = windowWidgetRootViews.begin();
        while(it != windowWidgetRootViews.end()){
            addChildNativeItem(*it);
            ++it;
        };
       if(menu) {
           HMENU hmenu = (HMENU)menu->getNativeBinding();
            SetMenu(hwnd,hmenu);
       };
       isReady = true;
    };
    void WinAppWindow::enable(){
        if(!IsWindowVisible(hwnd)){
            ShowWindow(hwnd,SW_SHOW);
        };
    };
    void WinAppWindow::disable(){
        if(IsWindowVisible(hwnd)){
            ShowWindow(hwnd,SW_HIDE);
        };
    };

    void WinAppWindow::initialDisplay(){
        ShowWindow(hwnd,SW_SHOW);
        UpdateWindow(hwnd);
    };
};

