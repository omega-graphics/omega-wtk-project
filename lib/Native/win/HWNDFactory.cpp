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
    ATOM HWNDFactory::registerWindow(LPCSTR class_name,WNDPROC proc_ptr){
        wndclassregistry.push_back(class_name);
        WNDCLASS ex;
        ex.lpszClassName = class_name;
        ex.lpfnWndProc = proc_ptr;
        ex.hIcon = nullptr;
        ex.style = CS_HREDRAW | CS_VREDRAW;
        ex.lpszMenuName = nullptr;
        return RegisterClass(&ex);

    };

};