
#include "pch.h"

#include "resource.h"

class TestApplication {
    static HWND m_hwnd;
    public:
    static int init(HINSTANCE hInst,int nCmdShow){
        int argc;
        LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(),&argc);
        LocalFree(argv);

        WNDCLASSEX wndclass = {0};
        wndclass.cbSize = sizeof(WNDCLASSEX);
        wndclass.style = CS_HREDRAW | CS_VREDRAW;
        wndclass.lpfnWndProc = (WNDPROC)WindowProc;
        wndclass.hInstance = hInst;
        wndclass.hCursor = LoadCursor(hInst,IDC_ARROW);
        wndclass.lpszClassName = "TestApplication";
        wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
        RegisterClassEx(&wndclass);

        m_hwnd = CreateWindow(
            wndclass.lpszClassName,
            "TestApplication",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            800,
            800,
            nullptr,
            nullptr,
            hInst,
            nullptr);

        ShowWindow(m_hwnd,nCmdShow);
        UpdateWindow(m_hwnd);

        HACCEL hAccelTable = LoadAccelerators(hInst,MAKEINTRESOURCE(IDC_TEST));

        MSG msg = {};
        while (GetMessage(&msg,nullptr,0,0))
        {
            // Process any messages in the queue.
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return msg.wParam;
    };  
    static HWND GetHwnd() { return m_hwnd; }
    protected:
        LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

             switch (message)
            {
            case WM_CREATE:
                {
                    // Save the DXSample* passed in to CreateWindow.
                    // LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
                    // SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
                }
                break;

            case WM_KEYDOWN:
                
                break;

            case WM_KEYUP:
                
                break;

            case WM_PAINT: {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd,&ps);
                EndPaint(hWnd,&ps);
                break;
            }
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }

    // Handle any messages the switch statement didn't.
    return 0;
    };
};

HWND TestApplication::m_hwnd = nullptr;


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
    return TestApplication::init(hInstance,nShowCmd);
};