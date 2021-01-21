#include "NativeItem_Win.h"
#include <windows.h>

namespace OmegaWTK {
    NativeItem * win_native_item(){
        HWND hwnd = nullptr;
        // HWND wind = CreateWindowA("", lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
        return (NativeItem *)hwnd;
    };
}
