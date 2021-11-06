#include "WinAppWindow.h"
#include "HWNDFactory.h"
#include "WinMenu.h"

#include <dwmapi.h>
#include <memory>
#include <windowsx.h>

#pragma comment(lib,"dwmapi.lib")

namespace OmegaWTK::Native::Win {
    WinAppWindow::WinAppWindow(Core::Rect & rect,NativeEventEmitter *emitter):HWNDItem(rect),isReady(false){
        this->event_emitter = emitter;
        // MessageBoxA(HWND_DESKTOP,"Creating WinAppWindow!","NOTE",MB_OK);
        atom = HWNDFactory::appFactoryInst->registerAppWindow();
        if(!atom){
            MessageBoxA(HWND_DESKTOP,"Failed to Register Desktop Window",NULL,MB_OK);
        };
        HWNDFactory::appFactoryInst->makeAppWindow(atom,"",rect,WS_OVERLAPPEDWINDOW,WS_EX_LAYERED,(void *)this);
        if(hwnd == NULL){
            MessageBoxA(HWND_DESKTOP,"Failed to Create Desktop Window",NULL,MB_OK);
        }
        currentDpi = GetDpiForWindow(hwnd);
        isTracking = false;
        hovered = false;
    };

    NativeItemPtr WinAppWindow::getRootView() {
        return (NativeItemPtr)this;
    }

    void WinAppWindow::addNativeItem(NativeItemPtr item){
       auto hwndItem = std::dynamic_pointer_cast<HWNDItem>(item);
       SetParent(hwndItem->hwnd,hwnd);
    };

    void WinAppWindow::setMenu(NM menu){
        this->menu = menu;
        auto nm = std::dynamic_pointer_cast<Win::WinMenu>(menu);
        SetMenu(hwnd,(HMENU)nm->getNativeBinding());
    }

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


//     LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
// {
//     // Get the point coordinates for the hit test.
//     POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

//     // Get the window rectangle.
//     RECT rcWindow;
//     GetWindowRect(hWnd, &rcWindow);

//     // Get the frame rectangle, adjusted for the style without a caption.
//     RECT rcFrame = { 0 };
//     AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

//     // Determine if the hit test is for resizing. Default middle (1,1).
//     USHORT uRow = 1;
//     USHORT uCol = 1;
//     bool fOnResizeBorder = false;

//     // Determine if the point is at the top or bottom of the window.
//     if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top)
//     {
//         fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
//         uRow = 0;
//     }
//     else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom)
//     {
//         uRow = 2;
//     }

//     // Determine if the point is at the left or right of the window.
//     if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left)
//     {
//         uCol = 0; // left side
//     }
//     else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right)
//     {
//         uCol = 2; // right side
//     }

//     // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
//     LRESULT hitTests[3][3] = 
//     {
//         { HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
//         { HTLEFT,       HTNOWHERE,     HTRIGHT },
//         { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
//     };

//     return hitTests[uRow][uCol];
// }

    BOOL WinAppWindow::ProcessWndMsgImpl(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *lr){
        *lr = 0;
        if(!DwmDefWindowProc(hwnd,uMsg,wParam,lParam,lr))
            // MessageBoxA(HWND_DESKTOP,"WinAppWindow Procedure","NOTE",MB_OK);
            switch (uMsg) {
                // case WM_CREATE :
                // {
                //     RECT rcClient;
                //     GetWindowRect(hWnd, &rcClient);

                //     // Inform the application of the frame change.
                //     SetWindowPos(hWnd, 
                //                 NULL, 
                //                 rcClient.left, rcClient.top,
                //                 rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,
                //                 SWP_FRAMECHANGED);
                //     break;
                // }
                
                // case WM_NCCALCSIZE : {
                //     MARGINS margins {-1};
                //     // margins.cxLeftWidth = 0;
                //     // margins.cxRightWidth = 0;
                //     // margins.cyTopHeight = 40;
                //     // margins.cyBottomHeight = 0;
                //     HRESULT hr = S_OK;
                //     hr = DwmExtendFrameIntoClientArea(hwnd,&margins);
                //     if (FAILED(hr))
                //     {
                //         *lr = -1;
                //     }
                    
                //     break;
                // }
                // case WM_NCHITTEST : {
                    
                //     // *lr = HitTestNCA(hwnd,wParam,lParam);
                    
                //     // if (*lr == HTNOWHERE)
                //     // {
                //     //     return FALSE;
                //     // }
                //     // break;
                // }
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
                        emitIfPossible((NativeEventPtr)new NativeEvent(NativeEvent::WindowWillClose,nullptr));
                    };
                    break;
                };
                case WM_SIZE : {
                    if(isReady) {
                        FLOAT scaleFactor = currentDpi/96.f;
                        UINT width = LOWORD(lParam);
                        UINT height = HIWORD(lParam);
                        updateAllHWNDPos(height,&raw_children);
                        wndrect = OmegaWTK::Core::Rect {OmegaGTE::GPoint2D {wndrect.pos.x,wndrect.pos.y},FLOAT(width)/scaleFactor,FLOAT(height)/scaleFactor};
                        auto params = new Native::WindowWillResize(wndrect);
                        emitIfPossible((NativeEventPtr)new NativeEvent(NativeEvent::WindowWillResize,params));
                    };
                    break;
                };
                // case WM_SIZING : {
                //     // if(isReady) {
                //     //     RECT rc = getClientRect();
                //     //     UINT height = rc.bottom - rc.top;
                //     //     updateAllHWNDPos(height,&raw_children);
                //     // };
                //     break;
                // };
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
                        nullptr,
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
            if(SetMenu(hwnd,hmenu) == FALSE){
                OMEGAWTK_DEBUG("Failed to Attach Menu to AppWindow");
            };
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

    void WinAppWindow::close(){
        if(IsWindow(hwnd))
            DestroyWindow(hwnd);
    };

    void WinAppWindow::initialDisplay(){
        auto it = windowWidgetRootViews.begin();
        while(it != windowWidgetRootViews.end()){
            auto item = std::dynamic_pointer_cast<HWNDItem>(*it);
            if(!IsWindowVisible(item->hwnd))
                ShowWindow(item->hwnd,SW_SHOW);
            ++it;
        };
        ShowWindow(hwnd,SW_SHOWDEFAULT);
        UpdateWindow(hwnd);
    };
    WinAppWindow::~WinAppWindow(){
        close();
    };
};

namespace OmegaWTK::Native {
    NWH make_native_window(Core::Rect &rect, NativeEventEmitter *emitter){
        return (NWH)new Win::WinAppWindow(rect,emitter);
    }
    
}

