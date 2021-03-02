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

class AppWindow;
class AppWindowManager;

    namespace Composition {

        class WindowStyle {
            Core::Vector<Core::UniquePtr<Visual>> visuals;
        public:
            
        };

        class WindowLayer : public Native::NativeEventEmitter, public Native::NativeLayer {
            Native::NWH native_window_ptr;
            Core::Rect & rect;
            friend class OmegaWTK::AppWindow;
            friend class OmegaWTK::AppWindowManager;
            void redraw();
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
        SharedHandle<Menu> menu;
        friend class AppWindowDelegate;
        friend class AppWindowManager;
        void _add_widget(SharedHandle<Widget> * widget);
    public:
        void setMenu(SharedHandle<Menu> & menu);
        UniqueHandle<Native::NativeFSDialog> openFSDialog();
        UniqueHandle<Native::NativeNoteDialog> openNoteDialog();
        template<class _Ty,std::enable_if_t<std::is_base_of_v<Widget,_Ty>,int> = 0>
        void addWidget(SharedHandle<_Ty> & widget){
            _add_widget((SharedHandle<Widget> *)&widget);
        };
        AppWindow(Core::Rect rect,AppWindowDelegate * delegate = nullptr);
    };

    class OMEGAWTK_EXPORT AppWindowManager {
        SharedHandle<AppWindow> rootWindow;
        public:
        AppWindowManager();
        void setRootWindow(SharedHandle<AppWindow> & handle);
        void displayRootWindow();
    };
    

    class OMEGAWTK_EXPORT AppWindowDelegate : public Native::NativeEventProcessor {
    private:
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class AppWindow;
    protected:
       AppWindow * window;
        virtual void windowWillClose(Native::NativeEventPtr event);
    };


}

#endif
