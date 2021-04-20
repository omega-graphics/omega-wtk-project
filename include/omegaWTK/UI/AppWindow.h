#include "omegaWTK/Core/Core.h"
#include "View.h"
#include "Menu.h"
#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Visual.h"
#include "omegaWTK/Native/NativeDialog.h"

#ifndef OMEGAWTK_UI_APPWINDOW_H
#define OMEGAWTK_UI_APPWINDOW_H

namespace OmegaWTK {
   

class AppWindow;
class AppWindowManager;

    class AppWindowDelegate;
    /**
     @brief A standard application window for attaching and displaying widgets. Similar to a Widget, it can be styled.
     @paragraph

    */
 class OMEGAWTK_EXPORT AppWindow : public Native::NativeEventEmitter {
        UniqueHandle<Composition::WindowLayer> layer;
        Composition::Compositor * compositor;
        SharedHandle<AppWindowDelegate> delegate;
        Core::Rect rect;
        Core::Vector<SharedHandle<Widget>> rootWidgets;
        SharedHandle<Menu> menu;
        SharedHandle<Composition::MenuStyle> menuStyle;
        friend class AppWindowDelegate;
        friend class AppWindowManager;
        void _add_widget(SharedHandle<Widget> * widget);
        void drawWidgets();
    public:
        void setMenu(SharedHandle<Menu> & menu);
        void setLayerStyle(SharedHandle<Composition::WindowStyle> & style);
        void setMenuStyle(SharedHandle<Composition::MenuStyle> & style);
        void close();
        SharedHandle<Native::NativeFSDialog> openFSDialog(const Native::NativeFSDialog::Descriptor & desc);
        SharedHandle<Native::NativeNoteDialog> openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc);
        template<class _Ty,std::enable_if_t<std::is_base_of_v<Widget,_Ty>,int> = 0>
        void addWidget(SharedHandle<_Ty> & widget){
            _add_widget((SharedHandle<Widget> *)&widget);
        };
        AppWindow(Core::Rect rect,AppWindowDelegate * delegate = nullptr);
    };
/**
 @brief Manages the displaying of AppWindows as well as the window heirarchy for a single application.
*/
 class OMEGAWTK_EXPORT  AppWindowManager {
        SharedHandle<AppWindow> rootWindow;
        void closeAllWindows();
        friend class AppInst;
        public:
        AppWindowManager();
        void setRootWindow(SharedHandle<AppWindow> & handle);
        SharedHandle<AppWindow> getRootWindow();
        void displayRootWindow();
    };
    
/**
 @brief An interface used for response code to a native event emitted by an AppWindow, for example when the window resizes.
 @paragraph


*/
class OMEGAWTK_EXPORT   AppWindowDelegate : public Native::NativeEventProcessor {
    private:
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class AppWindow;
    protected:
       AppWindow * window;
        virtual void windowWillClose(Native::NativeEventPtr event);
        virtual void windowWillResize(Core::Rect & nRect);
    };


}

#endif
