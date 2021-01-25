#include "NativePrivate/win/HWNDItem.h"
#include "NativePrivate/win/HWNDFactory.h"

namespace OmegaWTK::Native::Win {
    HWNDItem::HWNDItem(Core::Rect & rect,Type type):
        wnd_proc_ptr([&](HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) -> LRESULT{

            if(event_handler){
                event_handler->handleMessages(uMsg,wParam,lParam);
            };

            switch (uMsg) {
            case WM_CREATE :
                break;
            case WM_DESTROY:
                PostQuitMessage(0); 
                break;
            default:
                return DefWindowProc(hwnd,uMsg,wParam,lParam);
                break;
            }
            return 0;
        }),
        type(type){
            //TODO: Randomize Class Names!
        LPCSTR class_name = "WindowClass";
        HWNDFactory::appFactoryInst->registerWindow(class_name,wnd_proc_ptr.target<LRESULT CALLBACK(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)>());
        hwnd = HWNDFactory::appFactoryInst->makeWindow(class_name,"",rect,WS_CHILD | WS_TABSTOP,nullptr);
    };
    void HWNDItem::setHandler(WinEventHandler *eventHandler){
        event_handler = eventHandler;
    };
}