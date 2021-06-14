#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"
#include <Windows.h>

#ifndef OMEGAWTK_NATIVE_WIN_HWNDITEM_H
#define OMEGAWTK_NATIVE_WIN_HWNDITEM_H
namespace OmegaWTK::Native {
    namespace Win {
        class HWNDFactory;
        class HWNDItem : public NativeItem {
            public:
            HWND hwnd;
            bool enabled = false;
            bool isScrollView;
            protected:
            HWNDItem *parent;
            Core::Vector<HWND> raw_children;
            Core::Vector<HWNDItem *> children;
            UINT currentDpi;
            bool isTracking;
            bool hovered;
            virtual LRESULT ProcessWndMsg(UINT,WPARAM,LPARAM);
            virtual BOOL ProcessWndMsgImpl(HWND,UINT,WPARAM,LPARAM,LRESULT *);
            ATOM atom;
            void emitIfPossible(NativeEventPtr event);
            friend class HWNDFactory;
            void enable(){
                enabled = true;
                // ShowWindow(hwnd,SW_SHOWDEFAULT);
            };
            void disable(){
                enabled = false;
                // ShowWindow(hwnd,SW_HIDE);
            };
            void resize(Core::Rect &newRect);
            void addChildNativeItem(NativeItemPtr nativeItem);
            void removeChildNativeItem(NativeItemPtr nativeItem);
            public:
             /**
            Constructs a null HWNDItem! (Sets the Core::Rect only)
            */
            HWNDItem(Core::Rect & rect);
            Core::Rect wndrect;
            ATOM getAtom();
            HWND getHandle();
            bool isExtended();
            DWORD getStyle();
            DWORD getExtendedStyle();
            // void show(int nCmdShow);
            // void update();
            void destroy();
            RECT getClientRect();
            HDC getDCFromHWND();
            void *getBinding(){ return (void *)hwnd;};
            /// @name ScrollView Methods
            /// @{
            void toggleHorizontalScrollBar(bool & state);
            void toggleVerticalScrollBar(bool & state);
            void setClippedView(NativeItem *clippedView);
            /// @}
            typedef enum : OPT_PARAM {
                View,
                ScrollView
            } Type;
            private:
            Type type;
            public:
             /**
            Constructs/Registers an HWND and returns an HWNDItem!
            */
            HWNDItem(Core::Rect & rect,Type _type,HWNDItem *parent);
            ~HWNDItem(){};
        };
    };
};

#endif