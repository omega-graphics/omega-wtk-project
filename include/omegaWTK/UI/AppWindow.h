#include "omegaWTK/Core/Core.h"
#include "View.h"
#include "Menu.h"
#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Canvas.h"
#include "omegaWTK/Native/NativeDialog.h"
#include "Layout.h"

#ifndef OMEGAWTK_UI_APPWINDOW_H
#define OMEGAWTK_UI_APPWINDOW_H

namespace OmegaWTK {
   

class AppWindow;
class AppWindowManager;
class WidgetTreeHost;

class AppWindowDelegate;
/**
    @brief A standard application window for attaching and displaying widgets. Similar to a Widget, it can be styled.
    @paragraph
    In order to display any widgets on this window a WidgetTreeHost (@see WidgetTreeHost) must be created, 
    than attached to an instance of this class.
*/
 class OMEGAWTK_EXPORT AppWindow : public Native::NativeEventEmitter {
        /// A pointer to its compositor.
        Composition::Compositor *comp;
        /// Its Window Layer
        UniqueHandle<Composition::WindowLayer> layer;

        SharedHandle<AppWindowDelegate> delegate;
        /// A list of all attached widget tree hosts. 
        /// (Only needed to week widget tree hosts alive during application)
        OmegaCommon::Vector<SharedHandle<WidgetTreeHost>> widgetTreeHosts;

        Core::Rect rect;

        OmegaCommon::Vector<SharedHandle<Widget>> rootWidgets;

        SharedHandle<Menu> menu;

        // SharedHandle<Composition::MenuStyle> menuStyle;

        friend class AppWindowDelegate;
        friend class AppWindowManager;
        friend class WidgetTreeHost;
        void _add_widget(SharedHandle<Widget> * widget);


        void drawWidgets();
    public:
#if TARGET_WIN32
        struct SystemButton {
            Core::Rect rect;
            Composition::Color bkgrd;
            SharedImageHandle img;
            Composition::LayerAnimationController animController;
        };
        void setUseStandardSystemButtons(bool use);
        void setCloseButton(SharedHandle<SystemButton> & button);
        void setMaxmizeButton(SharedHandle<SystemButton> & button);
        void setMinimizeButton(SharedHandle<SystemButton> & button);

        void setMenuStyle(SharedHandle<Composition::MenuStyle> & style);
#endif

#if TARGET_MACOS
        void setCloseButtonPos(Core::Position pos); 
        void setMaximizeButtonPos(Core::Position pos); 
        void setMinimizeButtonPos(Core::Position pos); 
#endif
        
        void setMenu(SharedHandle<Menu> & menu);
        SharedHandle<Composition::WindowLayer> & getLayer();
        void close();
        SharedHandle<Native::NativeFSDialog> openFSDialog(const Native::NativeFSDialog::Descriptor & desc);
        SharedHandle<Native::NativeNoteDialog> openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc);
        
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
