#include "omegaWTK/Core/Core.h"

#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Native/NativeDialog.h"
#include "omegaWTK/Native/NativeTheme.h"

#ifndef OMEGAWTK_UI_APPWINDOW_H
#define OMEGAWTK_UI_APPWINDOW_H

namespace OmegaWTK {

namespace Composition {
    class WindowLayer;
    class ViewRenderTarget;
}

class Menu;
   
class AppWindow;
class AppWindowManager;
class WidgetTreeHost;
class Widget;

class AppWindowDelegate;
/**
    @brief A standard application window for attaching and displaying widgets. Similar to a Widget, it can be styled.
    @paragraph
    In order to display any widgets on this window a WidgetTreeHost must be created,
    than attached to an instance of this class.
*/
 class OMEGAWTK_EXPORT AppWindow : public Native::NativeEventEmitter,
                                    public Native::NativeThemeObserver {

        /// Its Window Layer
        UniqueHandle<Composition::WindowLayer> layer;

        SharedHandle<Composition::ViewRenderTarget> rootViewRenderTarget;

        Composition::CompositorClientProxy proxy;

        SharedHandle<AppWindowDelegate> delegate;
        /// A list of all attached widget tree hosts. 
        /// (Only needed to keep widget tree hosts alive during application)
        OmegaCommon::Vector<SharedHandle<WidgetTreeHost>> widgetTreeHosts;

        Core::Rect rect;

        SharedHandle<Menu> menu;

        // SharedHandle<Composition::MenuStyle> menuStyle;

        friend class AppWindowDelegate;
        friend class AppWindowManager;
        friend class WidgetTreeHost;

        void onThemeSet(Native::ThemeDesc &desc) override;
        void _add_widget(Widget *widget);
    public:
        OMEGACOMMON_CLASS("OmegaWTK.AppWindow")

        void add(Widget *widget);
        void setMenu(SharedHandle<Menu> & menu);
        void setEnableWindowHeader(bool enable);
#ifdef TARGET_WIN32
        SharedHandle<View> getExitButton();
        SharedHandle<View> getMaxmizeButton();
        SharedHandle<View> getMinimizeButton();
#endif
        void setTitle(OmegaCommon::StrRef title);
        SharedHandle<Composition::WindowLayer> & getLayer();
        void close();
        SharedHandle<Native::NativeFSDialog> openFSDialog(const Native::NativeFSDialog::Descriptor & desc);
        SharedHandle<Native::NativeNoteDialog> openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc);
        
        explicit AppWindow(Core::Rect rect,AppWindowDelegate * delegate = nullptr);
        ~AppWindow() override;
    };
/**
 @brief Manages the displaying of AppWindows as well as the window heirarchy for a single application.
*/
class OMEGAWTK_EXPORT  AppWindowManager : public Native::NativeThemeObserver {
        AppWindow * rootWindow;
        void closeAllWindows();
        friend class AppInst;
        void onThemeSet(Native::ThemeDesc &desc) override;
        public:
        AppWindowManager();
        void setRootWindow(AppWindow * handle);
        AppWindow * getRootWindow();
        void displayRootWindow();
        ~AppWindowManager() override = default;
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
