#include "omegaWTK/Core/Core.h"
#include "View.h"
#include "Menu.h"

#ifndef OMEGAWTK_UI_WINDOW_H
#define OMEGAWTK_UI_WINDOW_H

namespace OmegaWTK {
    namespace Native {
        class NativeWindow;
        class NativeFSDialog;
        class NativeNoteDialog;
        typedef NativeWindow *NWH;
    };

    namespace Composition {

        class WindowStyle {
            Core::Vector<Core::UniquePtr<Visual>> visuals;
        public:
            
        };

        class WindowLayer : public Native::NativeEventEmitter {
            Native::NWH native_window_ptr;
            Core::Rect & rect;
        public:
            WindowLayer(Core::Rect & rect,Native::NWH native_window_ptr);
        };

    };

    class AppWindowDelegate;
    class OMEGAWTK_EXPORT AppWindow : public Native::NativeEventEmitter {
        UniqueHandle<Composition::WindowLayer> layer;
        SharedHandle<AppWindowDelegate> delegate;
        Core::Rect rect;
        Core::Vector<SharedHandle<Widget>> rootWidgets;
        UniqueHandle<Menu> menu;
        friend class AppWindowDelegate;
    public:
        void setMenu(UniqueHandle<Menu> && menu);
        UniqueHandle<Native::NativeFSDialog> openFSDialog();
        UniqueHandle<Native::NativeNoteDialog> openNoteDialog();
        AppWindow(Core::Rect rect,AppWindowDelegate * delegate);
    };

    class AppWindowManager {
        UniqueHandle<AppWindow> rootWindow;
        public:
        void setRootWindow(UniqueHandle<AppWindow> && handle);
    };
    

    class OMEGAWTK_EXPORT AppWindowDelegate : public Native::NativeEventProcessor {
    private:
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class AppWindow;
    protected:
       AppWindow * window;
    public:
        AppWindowDelegate();
    };


}

#endif