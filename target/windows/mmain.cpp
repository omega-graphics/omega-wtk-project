//#include "resource.h"
#include <OmegaWTK.h>

#define MAX_LOADSTRING 100

// LPCSTR szTitle = "@APPNAME@";                
// LPCSTR szWindowClass = "@APPNAME@"; 
static const WORD MAX_CONSOLE_LINES = 500;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,  PSTR lpCmdLine, int nShowCmd){

    HRESULT hr;
    
    hr = CoInitializeEx(NULL,COINIT_MULTITHREADED);
    if(!SUCCEEDED(hr))
    {
        //Handle Error!
    }
   
    OmegaWTK::AppInst *appInst = new OmegaWTK::AppInst((void *)hInstance);
   
   
    int returnCode = omegaWTKMain(appInst);

    CoUninitialize();

    delete appInst;


    return returnCode;

};