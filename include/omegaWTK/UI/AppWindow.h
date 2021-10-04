#include "omegaWTK/Core/Core.h"
#include "View.h"
#include "Menu.h"
#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Canvas.h"
#include "omegaWTK/Native/NativeDialog.h"

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
    In order to display any widgets on this window a WidgetTreeHost must be created,
    than attached to an instance of this class.
*/
 class OMEGAWTK_EXPORT AppWindow : public Native::NativeEventEmitter, 
                                   public Composition::CompositorClient {

        /// Its Window Layer
        UniqueHandle<Composition::WindowLayer> layer;

        SharedHandle<AppWindowDelegate> delegate;
        /// A list of all attached widget tree hosts. 
        /// (Only needed to keep widget tree hosts alive during application)
        OmegaCommon::Vector<WidgetTreeHost *> widgetTreeHosts;

        Core::Rect rect;

        SharedHandle<Menu> menu;

        // SharedHandle<Composition::MenuStyle> menuStyle;

        friend class AppWindowDelegate;
        friend class AppWindowManager;
        friend class WidgetTreeHost;
        void _add_widget(Widget * widget);

        void commitRender();

        // void drawWidgets();
    public:
#if TARGET_WIN32
        struct SystemButton {
            Core::Rect rect;
            Composition::Color bkgrd;
            SharedHandle<Media::BitmapImage> img;
            Composition::AnimationController animController;
        };
        void setUseStandardSystemButtons(bool use);
        void setCloseButton(SharedHandle<SystemButton> & button);
        void setMaxmizeButton(SharedHandle<SystemButton> & button);
        void setMinimizeButton(SharedHandle<SystemButton> & button);
#endif

#if TARGET_MACOS
        void setCloseButtonPos(Core::Position pos); 
        void setMaximizeButtonPos(Core::Position pos); 
        void setMinimizeButtonPos(Core::Position pos); 
#endif
        void add(Widget *widget);
        void setMenu(SharedHandle<Menu> & menu);
        SharedHandle<Composition::WindowLayer> & getLayer();
        void close();
        SharedHandle<Native::NativeFSDialog> openFSDialog(const Native::NativeFSDialog::Descriptor & desc);
        SharedHandle<Native::NativeNoteDialog> openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc);
        
        explicit AppWindow(Core::Rect rect,AppWindowDelegate * delegate = nullptr);
        ~AppWindow();
    };
/**
 @brief Manages the displaying of AppWindows as well as the window heirarchy for a single application.
*/
 class OMEGAWTK_EXPORT  AppWindowManager {
        AppWindow * rootWindow;
        void closeAllWindows();
        friend class AppInst;
        public:
        AppWindowManager();
        void setRootWindow(AppWindow * handle);
        AppWindow * getRootWindow();
        void displayRootWindow();
    };
    
/**
 @brief An interface used for response code to a native event emitted by an AppWindow, for example when the window resizes.
 @paragraph


*/
INTERFACE OMEGAWTK_EXPORT  AppWindowDelegate : public Native::NativeEventProcessor {
    private:
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class AppWindow;
    protected:
       AppWindow * window;
         INTERFACE_METHOD void windowWillClose(Native::NativeEventPtr event);
        INTERFACE_METHOD void windowWillResize(Core::Rect & nRect);
    };


}

#endif
