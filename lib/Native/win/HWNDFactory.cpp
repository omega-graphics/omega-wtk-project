#include "HWNDFactory.h"
#include "NativePrivate/win/HWNDItem.h"

namespace OmegaWTK::Native::Win {

    int windowID = 0;
    std::string str = "OmegaWTKWinView_";

    HWNDFactory * HWNDFactory::appFactoryInst;

    HWNDFactory::HWNDFactory(HINSTANCE hinst,HWND rootHwnd):rootWindow(rootHwnd),hInst(hinst){
       
    };

    HWNDFactory::~HWNDFactory(){};

    void * setHWNDUserData(HWND hwnd,void *data){
        return (void *)SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)data);
    };

    void *getHWNDUserData(HWND hwnd){
        return (void *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
    };

    HWND HWNDFactory::getRootWnd(){ return rootWindow;};

    LRESULT HWNDFactory::WndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
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
    HWND HWNDFactory::makeWindow(ATOM atom,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,HWND parent,DWORD ext_style){
        HWND wind_parent;
        if(parent != nullptr)
            wind_parent = parent;
        else 
            wind_parent = rootWindow;

        UINT dpi = GetDpiForWindow(wind_parent);

        // MessageBox(GetForegroundWindow(),(std::string("DPI:") + std::to_string(dpi)).c_str(),"Note",MB_OK);

        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        /// Windows Coordinate system fix!
        RECT rc;
        GetClientRect(wind_parent,&rc);
        // std::string s3 = "RECT for App Window:" + std::to_string(rc.left) + ",T:" + std::to_string(rc.top) + ",R:" + std::to_string(rc.right) + ",B:" + std::to_string(rc.bottom);
        // MessageBox(GetForegroundWindow(),s3.c_str(),"NOTE",MB_OK);
        
        // unsigned wndHeight = rc.bottom - rc.top;
        // unsigned height = rect.dimen.minHeight * scaleFactor;
        HWND hwnd = CreateWindowA(MAKEINTATOM(atom),name,base_style,rect.pos.x,(rc.bottom - (rect.dimen.minHeight) * scaleFactor) - rect.pos.y,rect.dimen.minWidth * scaleFactor,rect.dimen.minHeight * scaleFactor,wind_parent,NULL,hInst,custom_params);
        if(parent == nullptr)
            all_hwnds.push_back(hwnd);
        return hwnd;
        #undef DEFAULT_DPI
        
    };
    ATOM HWNDFactory::registerWindow(){

        str.append(std::to_string(windowID));
        ++windowID;

        wndclassregistry.push_back(str);
        WNDCLASSEX ex;
        ex.cbSize = sizeof(ex);
        ex.lpszClassName = str.c_str();
        ex.lpszMenuName = NULL;
        ex.cbClsExtra = 0;
        ex.cbWndExtra = 0;
        ex.hbrBackground = (HBRUSH)COLOR_WINDOW+1;
        ex.hInstance = hInst;
        ex.hCursor = LoadCursor(NULL,IDC_ARROW);
        ex.hIcon = NULL;
        ex.hIconSm = NULL;
        ex.lpfnWndProc = WndProc;
        ex.style = CS_HREDRAW | CS_VREDRAW;
        return RegisterClassEx(&ex);

    };

};