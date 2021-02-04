#include "HWNDFactory.h"
#include "NativePrivate/win/HWNDItem.h"

namespace OmegaWTK::Native::Win {

    int windowID = 0;
    std::string str = "OmegaWTKWinView_";

    HWNDFactory * HWNDFactory::appFactoryInst = nullptr;

    HWNDFactory::HWNDFactory(HINSTANCE hinst){
         hInst = hinst;
        appFactoryInst = this;
    };

    HWNDFactory::~HWNDFactory(){};

    void * setHWNDUserData(HWND hwnd,void *data){
        return (void *)SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)data);
    };

    void *getHWNDUserData(HWND hwnd){
        return (void *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
    };

    CALLBACK LRESULT HWNDFactory::WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
        HWNDItem *hwndItem;
        if(uMsg == WM_NCCREATE){
            CREATESTRUCT *ctstrt = (CREATESTRUCT *)lParam;
            hwndItem = (HWNDItem *)ctstrt->lpCreateParams;
            setHWNDUserData(hwnd,(void *)hwndItem);
            hwndItem->hwnd = hwnd;
        }
        else {
            hwndItem = (HWNDItem *)getHWNDUserData(hwnd);
        };
        
        return hwndItem->ProcessWndMsg(uMsg,wParam,lParam);
    };

    void HWNDFactory::setRootWindowAndHINST(HWND root,HINSTANCE hinst){
        rootWindow = root;
        hInst = hinst;
    };
    HWND HWNDFactory::makeWindow(ATOM atom,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,DWORD ext_style){
        return CreateWindowEx(ext_style,reinterpret_cast<LPCSTR>(atom),name,base_style,rect.pos.x,rect.pos.y,rect.dimen.minWidth,rect.dimen.minHeight,nullptr,nullptr,hInst,custom_params);
    };
    ATOM HWNDFactory::registerWindow(){

        str.append(std::to_string(windowID));
        ++windowID;

        wndclassregistry.push_back(str);
        WNDCLASS ex;
        ex.lpszClassName = str.c_str();
        ex.lpfnWndProc = WndProc;
        ex.style = CS_HREDRAW | CS_VREDRAW;
        return RegisterClass(&ex);

    };

};