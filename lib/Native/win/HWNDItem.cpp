#include "NativePrivate/win/HWNDItem.h"
#include "HWNDFactory.h"
#include "NativePrivate/win/WinEvent.h"
#include <windowsx.h>
#include <iostream>

namespace OmegaWTK::Native::Win {
    HWNDItem::HWNDItem(Core::Rect & rect,Type type):wndrect(rect){
        std::cout << "Registering Window!" << std::endl;
        atom = HWNDFactory::appFactoryInst->registerWindow();
        if(!atom)
        {
            MessageBox(HWNDFactory::appFactoryInst->getRootWnd(),"Failed to Register HWNDItem Window!",NULL,MB_OKCANCEL);
        }
        HWNDFactory::appFactoryInst->makeWindow(atom,"",wndrect,WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,(void *)this);
    };
    LRESULT HWNDItem::ProcessWndMsg(UINT u_int,WPARAM wParam,LPARAM lParam){
        LRESULT result = 0;
        if(u_int == WM_NCDESTROY){
            hwnd = nullptr;
        };
        if(!ProcessWndMsgImpl(hwnd,u_int,wParam,lParam,&result))
            result = DefWindowProcA(hwnd,u_int,wParam,lParam);
        return result;
    };
    BOOL HWNDItem::ProcessWndMsgImpl(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT *lresult){
        BOOL rc = TRUE;
        NativeEventPtr event = nullptr;
        POINT pt;
        /// Set LRESULT to 0 (Assuming the message will be processed)
        *lresult = 0;

        switch (uMsg) {
        // case WM_LBUTTONDOWN : {
        //     pt.x = GET_X_LPARAM(lParam);
        //     pt.y = GET_Y_LPARAM(lParam);
        //     event = button_event_to_native_event(NativeEvent::LMouseDown,&pt);
        //     break;
        // };
        // case WM_LBUTTONUP : {
        //     pt.x = GET_X_LPARAM(lParam);
        //     pt.y = GET_Y_LPARAM(lParam);
        //     event = button_event_to_native_event(NativeEvent::LMouseUp,&pt);
        //     break;
        // };
        // case WM_RBUTTONDOWN : {
        //     pt.x = GET_X_LPARAM(lParam);
        //     pt.y = GET_Y_LPARAM(lParam);
        //     event = button_event_to_native_event(NativeEvent::RMouseDown,&pt);
        //     break;
        // };
        // case WM_RBUTTONUP : {
        //     pt.x = GET_X_LPARAM(lParam);
        //     pt.y = GET_Y_LPARAM(lParam);
        //     event = button_event_to_native_event(NativeEvent::RMouseUp,&pt);
        //     break;
        // };
        // case WM_PAINT : {
        //     PAINTSTRUCT ps;
        //     HDC hdc = BeginPaint(hwnd,&ps);

        //     EndPaint(hwnd,&ps);
        //     break;
        // };
        default:
           rc = FALSE;
           break;
        }

        if(event != nullptr)
            emitIfPossible(event);
        
        return rc;
    };
    void HWNDItem::emitIfPossible(NativeEventPtr event){
        if(hasEventEmitter())
            sendEventToEmitter(event);
    };
    bool HWNDItem::isExtended(){
        return false;
    };
    HWND HWNDItem::getHandle(){
        return hwnd;
    };
    DWORD HWNDItem::getStyle(){
        return GetWindowLongPtr(hwnd,GWL_STYLE);
    };
    void HWNDItem::show(int nCmdShow){
        ShowWindow(hwnd,nCmdShow);
    };
    void HWNDItem::update(){
        UpdateWindow(hwnd);
    };
    RECT HWNDItem::getClientRect(){
        RECT r;
        GetClientRect(hwnd,&r);
        return r;
    };
    HDC HWNDItem::getDCFromHWND(){
        return GetDC(hwnd);
    };
}