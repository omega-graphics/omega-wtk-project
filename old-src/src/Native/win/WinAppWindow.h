#include "NativePrivate/win/HWNDItem.h"
#include "omegaWTK/Native/NativeWindow.h"

#ifndef OMEGAWTK_NATIVE_WIN_WINAPPWINDOW_H
#define OMEGAWTK_NATIVE_WIN_WINAPPWINDOW_H

namespace OmegaWTK::Native::Win {
    class WinAppWindow : public NativeWindow, public HWNDItem {
        bool isReady;
        public:
        void enable();
        void disable();
        void attachWidgets();
        void initialDisplay();
        void close();
        void addNativeItem(NativeItemPtr item);
        LRESULT ProcessWndMsg(UINT,WPARAM,LPARAM);
        BOOL ProcessWndMsgImpl(HWND, UINT, WPARAM, LPARAM, LRESULT *);
        WinAppWindow(Core::Rect & rect,NativeEventEmitter *emitter);
        ~WinAppWindow();
    };
};

#endif