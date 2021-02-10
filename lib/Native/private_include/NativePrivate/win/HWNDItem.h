#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"
#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_HWNDITEM_H
#define OMEGAWTK_NATIVE_WIN_HWNDITEM_H
namespace OmegaWTK::Native {
    namespace Win {
        class HWNDFactory;
        class HWNDItem : public NativeItem {
            HWND hwnd;
            Core::Rect wndrect;
            UINT currentDpi;
            LRESULT ProcessWndMsg(UINT,WPARAM,LPARAM);
            BOOL ProcessWndMsgImpl(HWND,UINT,WPARAM,LPARAM,LRESULT *);
            ATOM atom;
            void emitIfPossible(NativeEventPtr event);
            friend class HWNDFactory;
            public:
            ATOM getAtom();
            HWND getHandle();
            bool isExtended();
            DWORD getStyle();
            DWORD getExtendedStyle();
            void show(int nCmdShow);
            void update();
            void destroy();
            RECT getClientRect();
            HDC getDCFromHWND();
            void *getBinding(){ return (void *)hwnd;};
            typedef enum : OPT_PARAM {
                View,
                PushButton,
            } Type;
            private:
            Type type;
            public:
            HWNDItem(Core::Rect & rect,Type _type,HWNDItem *parent);
            ~HWNDItem(){};
        };
    };
};

#endif