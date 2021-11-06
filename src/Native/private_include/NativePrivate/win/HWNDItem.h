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
            OmegaCommon::Vector<HWND> raw_children;
            OmegaCommon::Vector<SharedHandle<HWNDItem>> children;
            UINT currentDpi;
            bool isTracking;
            bool hovered;
            virtual LRESULT ProcessWndMsg(UINT,WPARAM,LPARAM);
            virtual BOOL ProcessWndMsgImpl(HWND,UINT,WPARAM,LPARAM,LRESULT *);
            ATOM atom;
            void emitIfPossible(NativeEventPtr event);
            friend class HWNDFactory;
            void enable() override{
                enabled = true;
                // ShowWindow(hwnd,SW_SHOWDEFAULT);
            };
            void disable() override{
                enabled = false;
                // ShowWindow(hwnd,SW_HIDE);
            };
            void resize(const Core::Rect &newRect) override;
            void addChildNativeItem(NativeItemPtr nativeItem) override;
            void removeChildNativeItem(NativeItemPtr nativeItem) override;
            public:
             /**
            Constructs a null HWNDItem! (Sets the Core::Rect only)
            */
            HWNDItem(Core::Rect & rect);
            Core::Rect wndrect;
            Core::Rect & getRect() override {
                return wndrect;
            }
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
            void *getBinding() override{ return (void *)hwnd;};
            /// @name ScrollView Methods
            /// @{
            void toggleHorizontalScrollBar(bool & state) override;
            void toggleVerticalScrollBar(bool & state) override;
            void setClippedView(SharedHandle<NativeItem> clippedView) override;
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
            ~HWNDItem(){
                if(IsWindow(hwnd)){
                    DestroyWindow(hwnd);
                }
            };
        };
    };
};

#endif