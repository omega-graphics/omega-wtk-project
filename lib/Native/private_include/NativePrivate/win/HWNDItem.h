#include "omegaWTK/Native/NativeItem.h"
#include "WinEvent.h"
#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_HWNDITEM_H
#define OMEGAWTK_NATIVE_WIN_HWNDITEM_H
namespace OmegaWTK::Native {
    namespace Win {
        class HWNDItem : public NativeItem {
            HWND hwnd;
            WinEventHandler *event_handler  = nullptr;
            friend class WinEventHandler;
            void setHandler(WinEventHandler *eventHandler);
            std::function<CALLBACK LRESULT(HWND,UINT,WPARAM,LPARAM)> wnd_proc_ptr;
            public:
            typedef enum : OPT_PARAM {
                View,
                PushButton,
            } Type;
            private:
            Type type;
            public:
            HWNDItem(Core::Rect & rect,Type _type);
            ~HWNDItem(){};
        };
    };
};

#endif