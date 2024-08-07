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
OMEGACOMMON_SHARED_CLASS(AppWindow);
class AppWindowManager;
class WidgetTreeHost;
class Widget;
OMEGACOMMON_SHARED_CLASS(Widget);

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

        void add(WidgetPtr widget);

    #ifndef TARGET_MOBILE
        void setMenu(SharedHandle<Menu> & menu);
        void setEnableWindowHeader(bool enable);
    #endif
#ifdef TARGET_WIN32
        SharedHandle<View> getExitButton();
        SharedHandle<View> getMaxmizeButton();
        SharedHandle<View> getMinimizeButton();
#endif
        void setTitle(OmegaCommon::StrRef title);
        SharedHandle<Composition::WindowLayer> & getLayer();
        void close();


        #ifdef TARGET_MOBILE

        SharedHandle<Native::NativeFS> openFSDialog(const Native::NativeFSDialog::Descriptor & desc);
        SharedHandle<Native::NativeNoteDialog> openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc);

        #else

        SharedHandle<Native::NativeFSDialog> openFSDialog(const Native::NativeFSDialog::Descriptor & desc);
        SharedHandle<Native::NativeNoteDialog> openNoteDialog(const Native::NativeNoteDialog::Descriptor & desc);

        #endif
        
        explicit AppWindow(Core::Rect rect,AppWindowDelegate * delegate = nullptr);
        ~AppWindow() override;
    };
/**
 @brief Manages the displaying of AppWindows as well as the window heirarchy for a single application.
 Desktop Apps: All windows are seperate entiityes
 Mobile Apps: All windows are share the same screen. (The transition and position is determined by)
*/
class OMEGAWTK_EXPORT  AppWindowManager : public Native::NativeThemeObserver {
        AppWindowPtr rootWindow;

        OmegaCommon::Vector<AppWindowPtr> windows;

        void closeAllWindows();

        friend class AppInst;
        void onThemeSet(Native::ThemeDesc &desc) override;
        public:
        /**
         * @brief  USE make<> to create this.
         * 
         */
        AppWindowManager();

        typedef unsigned WindowIndex;

        /**
         * @brief Add an AppWindow to the manager (No priority for it. 
         The order in which it was placed determines the priority.)
         * 
         * @param handle 
         * @return The index of the window
         */
        WindowIndex addWindow(AppWindowPtr handle);

        /**
         * @brief Set the Root Window object
         * 
         * @param handle 
         */
        void setRootWindow(AppWindowPtr handle);

        AppWindowPtr getRootWindow();

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

#ifdef TARGET_MOBILE
class OMEGAWTK_EXPORT StoryBoard {

};
#endif

#endif
