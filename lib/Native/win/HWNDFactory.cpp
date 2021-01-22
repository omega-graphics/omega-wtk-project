#include "NativePrivate/win/HWNDFactory.h"

namespace OmegaWTK::Native::Win {
    void HWNDFactory::setRootWindowAndHINST(HWND root,HINSTANCE hinst){
        rootWindow = root;
        hInst = hinst;
    };
    void HWNDFactory::setCurrentParent(HWND parent){
        currentParent = parent;
    };
    HWND HWNDFactory::makeWindow(LPCSTR class_name,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params){
        return CreateWindow(class_name,name,base_style,rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight,currentParent,nullptr,hInst,custom_params);
    };
    ATOM HWNDFactory::registerWindow(){
        WNDCLASSEX ex;
        ex.cbSize = sizeof(WNDCLASSEX);
        ex.hInstance = hInst;
        ex.hbrBackground = (HBRUSH)COLOR_WINDOW;
        ex.style = CS_HREDRAW | CS_VREDRAW;
        return RegisterClassEx(&ex);
    };

};