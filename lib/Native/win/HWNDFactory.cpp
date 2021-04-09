#include "HWNDFactory.h"
#include "NativePrivate/win/HWNDItem.h"
#include "WinAppWindow.h"
#include "omegaWTK/Native/NativeApp.h"

#pragma comment(lib,"gdi32.lib")

namespace OmegaWTK::Native::Win {

    void updateAllHWNDPos(UINT root_wnd_height,Core::Vector<HWND> * hwnds_to_update){
     RECT rc;
     auto it = hwnds_to_update->begin();
     while(it != hwnds_to_update->end()){
         HWND hwnd = *it;
         rc = __get_hwnd_real_coords(hwnd);
         UINT dpi = GetDpiForWindow(hwnd);
         FLOAT scaleFactor = FLOAT(dpi)/96.f;
        //  auto str = std::string("Coords:{") + "l:" + std::to_string(rc.left) + ",b:" + std::to_string(rc.bottom) + ",r:" + std::to_string(rc.right) + ",t:" + std::to_string(rc.top) + "}";
         
        //  MessageBoxA(GetForegroundWindow(),str.c_str(),"NOTE",MB_OK);
         auto h = rc.top - rc.bottom;
         auto w = rc.right - rc.left;

        //  auto n_str = std::string("RECT {") + "x:" + std::to_string(rc.left * scaleFactor) + ",y:" + std::to_string(root_wnd_height - (rc.top* scaleFactor)) + ",w:" + std::to_string(w * scaleFactor) + ",h:" + std::to_string(h* scaleFactor) + "}";
        //   MessageBoxA(GetForegroundWindow(),n_str.c_str(),"NOTE",MB_OK);

         SetWindowPos(hwnd,hwnd,rc.left * scaleFactor,(root_wnd_height - (rc.top* scaleFactor)),w * scaleFactor,h* scaleFactor,SWP_NOZORDER | SWP_NOACTIVATE);
         ++it;
     };
 };

    int windowID = 0;
    std::string str = "OmegaWTKWinView_";

    HWNDFactory * HWNDFactory::appFactoryInst;

    HWNDFactory::HWNDFactory(HINSTANCE hinst):hInst(hinst){
       
    };

    HWNDFactory::~HWNDFactory(){};

    void * setHWNDUserData(HWND hwnd,void *data){
        return (void *)SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)data);
    };

    void *getHWNDUserData(HWND hwnd){
        return (void *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
    };

    /// HWND HWNDFactory::getRootWnd(){ return rootWindow;};

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

        if(!hwndItem)
            return 0;

       // MessageBoxA(HWND_DESKTOP,"Std WndProc","NOTE",MB_OK);
        
        return hwndItem->ProcessWndMsg(uMsg,wParam,lParam);
    };

    LRESULT HWNDFactory::WndProc2(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
        WinAppWindow *hwndItem = nullptr;
        if(uMsg == WM_NCCREATE){
            // MessageBoxA(HWND_DESKTOP,"WM_NCCREATE","NOTE",MB_OK);
            CREATESTRUCT *ctstrt = (CREATESTRUCT *)lParam;
            hwndItem = (WinAppWindow *)ctstrt->lpCreateParams;
            setHWNDUserData(hwnd,(void *)hwndItem);
            hwndItem->hwnd = hwnd;
        }
        else {
            hwndItem = (WinAppWindow *)getHWNDUserData(hwnd);
        };

        if(!hwndItem)
            return 0;
        
        // MessageBoxA(HWND_DESKTOP,"Std WndProc","NOTE",MB_OK);
        
        return hwndItem->ProcessWndMsg(uMsg,wParam,lParam);
    };

    void HWNDFactory::setRootWindowAndHINST(HWND root,HINSTANCE hinst){
        hInst = hinst;
    };
    HWND HWNDFactory::makeWindow(ATOM atom,LPCSTR name,Core::Rect rect,DWORD base_style,LPVOID custom_params,HWND parent,DWORD ext_style){
        HWND wind_parent;
        if(parent != nullptr)
            wind_parent = parent;
        else 
            wind_parent = GetForegroundWindow();

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
        HWND hwnd = CreateWindowExA(ext_style,MAKEINTATOM(atom),name,base_style,rect.pos.x *scaleFactor,(rc.bottom - (rect.dimen.minHeight) * scaleFactor) - (rect.pos.y * scaleFactor),rect.dimen.minWidth * scaleFactor,rect.dimen.minHeight * scaleFactor,wind_parent,NULL,hInst,custom_params);
        // UpdateWindow(hwnd);
        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.SourceConstantAlpha = 255;
        blend.AlphaFormat = AC_SRC_ALPHA;
        HDC hdcScreen = GetDC(NULL);
        HDC hdcMem = CreateCompatibleDC(hdcScreen);
        UpdateLayeredWindow(hwnd,hdcScreen,NULL,NULL,hdcMem,NULL,RGB(0xFF,0xFF,0xFF),&blend,ULW_ALPHA);
        DeleteDC(hdcMem);
        // ShowWindow(hwnd,SW_SHOWDEFAULT);
        return hwnd;
        #undef DEFAULT_DPI
        
    };
    HWND HWNDFactory::makeAppWindow(ATOM atom,LPCSTR name,Core::Rect & rect,DWORD base_style,DWORD ext_style,LPVOID custom_params){
        RECT rc;
        GetClientRect(HWND_DESKTOP,&rc);
        UINT dpi = GetDpiFromDpiAwarenessContext(GetThreadDpiAwarenessContext());
        // MessageBox(HWND_DESKTOP,"Making Window from Atom","NOTE",MB_OK);
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        HWND hwnd = CreateWindowA(MAKEINTATOM(atom),name,base_style,CW_USEDEFAULT,CW_USEDEFAULT,int(rect.dimen.minWidth),int(rect.dimen.minHeight),HWND_DESKTOP,NULL,hInst,custom_params);
        // MessageBox(HWND_DESKTOP,"Created Window!","NOTE",MB_OK);
        return hwnd;
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
        ex.hbrBackground = (HBRUSH)COLOR_WINDOW+2;
        ex.hInstance = hInst;
        ex.hCursor = LoadCursor(NULL,IDC_ARROW);
        ex.hIcon = NULL;
        ex.hIconSm = NULL;
        ex.lpfnWndProc = WndProc;
        ex.style = CS_HREDRAW | CS_VREDRAW;
        return RegisterClassEx(&ex);

    };

    ATOM HWNDFactory::registerAppWindow(){
        str.append(std::to_string(windowID));
        ++windowID;

        wndclassregistry.push_back(str);
        WNDCLASSEX ex;
        ex.cbSize = sizeof(ex);
        ex.lpszClassName = str.c_str();
        ex.lpszMenuName = NULL;
        ex.cbClsExtra = 0;
        ex.cbWndExtra = 0;
        ex.hbrBackground = (HBRUSH)COLOR_WINDOW;
        ex.hInstance = hInst;
        ex.hCursor = LoadCursor(NULL,IDC_ARROW);
        ex.hIcon = NULL;
        ex.hIconSm = NULL;
        ex.lpfnWndProc = WndProc2;
        ex.style = CS_HREDRAW | CS_VREDRAW;
        return RegisterClassEx(&ex);
    };

};