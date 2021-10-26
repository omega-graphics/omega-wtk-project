#include "NativePrivate/win/HWNDItem.h"
#include "omegaWTK/Native/NativeWindow.h"

#ifndef OMEGAWTK_NATIVE_WIN_WINAPPWINDOW_H
#define OMEGAWTK_NATIVE_WIN_WINAPPWINDOW_H

namespace OmegaWTK::Native::Win {
    class WinAppWindow : public NativeWindow, public HWNDItem {
        bool isReady;
        public:
        NativeItemPtr getRootView() override;
        void enable() override; 
        void disable() override;
        void attachWidgets();
        void initialDisplay() override;
        void close() override;
        void addNativeItem(NativeItemPtr item) override;
        LRESULT ProcessWndMsg(UINT,WPARAM,LPARAM) override;
        BOOL ProcessWndMsgImpl(HWND, UINT, WPARAM, LPARAM, LRESULT *) override;
        WinAppWindow(Core::Rect & rect,NativeEventEmitter *emitter);
        ~WinAppWindow();
    };
};

#endif