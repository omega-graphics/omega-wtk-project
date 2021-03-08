#include "NativePrivate/win/HWNDItem.h"
#include "HWNDFactory.h"
#include "NativePrivate/win/WinEvent.h"
#include <windowsx.h>
#include <iostream>

namespace OmegaWTK::Native::Win {
    HWNDItem::HWNDItem(Core::Rect & rect):wndrect(rect){
        /// Passes implemenation to children!
    };
    HWNDItem::HWNDItem(Core::Rect & rect,Type type,HWNDItem *parent):wndrect(rect){
        std::cout << "Registering Window!" << std::endl;
        atom = HWNDFactory::appFactoryInst->registerWindow();
        if(!atom)
        {
            // MessageBox(HWND_DESKTOP,"Failed to Register HWNDItem Window!",NULL,MB_OKCANCEL);
        }
        else {
            // MessageBoxA(HWND_DESKTOP,"Registered HWNDItem Window!","NOTE",MB_OK);
        };
        if(parent)
            HWNDFactory::appFactoryInst->makeWindow(atom,"",wndrect,WS_CHILD,(void *)this,parent->hwnd,WS_EX_LAYERED);
        else 
            HWNDFactory::appFactoryInst->makeWindow(atom,"",wndrect,WS_CHILD,(void *)this,nullptr,WS_EX_LAYERED);

        if(hwnd == NULL){
            // MessageBoxA(HWND_DESKTOP,"Failed to Create HWNDItem Window!","NOTE",MB_OK);
        };
        currentDpi = GetDpiForWindow(hwnd);
        isTracking = false;
        hovered = false;
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
        POINT pt;
        /// Set LRESULT to 0 (Assuming the message will be processed)
        *lresult = 0;

        switch (uMsg) {
        // case WM_DPICHANGED_AFTERPARENT : {

        //     UINT newDpi = GetDpiForWindow(hwnd);

        //     // MessageBox(GetForegroundWindow(),(std::string("NEW DPI:") + std::to_string(newDpi) + "\nOLD DPI" + std::to_string(currentDpi)).c_str(),"Note",MB_OK);

        //     FLOAT scaleFactor = FLOAT(newDpi)/currentDpi;

        //     RECT parentRect;
        //     GetClientRect(GetParent(hwnd),&parentRect);

        //     UINT newParentWndHeight = parentRect.bottom - parentRect.top;
            
        //     RECT rc = getClientRect();
        //     SetWindowPos(hwnd,hwnd,rc.left,(newParentWndHeight - (rc.bottom * scaleFactor)),(rc.bottom - rc.top) * scaleFactor,(rc.right - rc.left) * scaleFactor,SWP_NOZORDER | SWP_NOACTIVATE);

        //     currentDpi = newDpi;
        //     break;
        // }
        case WM_MOUSEMOVE : {
            if(!isTracking && !hovered){
                TRACKMOUSEEVENT ev;
                ev.cbSize = sizeof(ev);
                ev.hwndTrack = hwnd;
                ev.dwFlags  = TME_HOVER | TME_LEAVE;
                ev.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&ev);
                isTracking = true;
            };
            break;
        }
        case WM_MOUSEHOVER : {
            isTracking = false;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            emitIfPossible(button_event_to_native_event(NativeEvent::CursorEnter,&pt));
            hovered = true;
            break;
        };
        case WM_MOUSELEAVE : {
            isTracking = false;
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            emitIfPossible(button_event_to_native_event(NativeEvent::CursorExit,&pt));
            hovered = false;
            break;
        };
        case WM_LBUTTONDOWN : {
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            emitIfPossible(button_event_to_native_event(NativeEvent::LMouseDown,&pt));
            break;
        };
        case WM_LBUTTONUP : {
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            emitIfPossible(button_event_to_native_event(NativeEvent::LMouseUp,&pt));
            break;
        };
        case WM_RBUTTONDOWN : {
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            emitIfPossible(button_event_to_native_event(NativeEvent::RMouseDown,&pt));
            break;
        };
        case WM_RBUTTONUP : {
            pt.x = GET_X_LPARAM(lParam);
            pt.y = GET_Y_LPARAM(lParam);
            emitIfPossible(button_event_to_native_event(NativeEvent::RMouseUp,&pt));
            break;
        };
        case WM_PAINT : {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd,&ps);

            // layerTreelimb->redraw();

            EndPaint(hwnd,&ps);
            break;
        };
        default:
           rc = FALSE;
           break;
        }
        
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
    void HWNDItem::addChildNativeItem(NativeItem *nativeItem){
        children.push_back(nativeItem);
        HWND child = ((HWNDItem *)nativeItem)->hwnd;
        raw_children.push_back(child);
        SetParent(child,hwnd);
    };
    void HWNDItem::removeChildNativeItem(NativeItem * nativeItem){
        auto _ni_ptr_it = children.begin();
        auto _hwnd_it = raw_children.begin();
        while(_ni_ptr_it != children.end() && _hwnd_it != raw_children.end()){
            auto _native_item = *_ni_ptr_it;
            if(_native_item == nativeItem){
                children.erase(_ni_ptr_it);
                raw_children.erase(_hwnd_it);
                HWND child = ((HWNDItem *)nativeItem)->hwnd;
                SetParent(child,NULL);
                return;
                break;
            }; 
            ++_ni_ptr_it;
            ++_hwnd_it;
        };
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