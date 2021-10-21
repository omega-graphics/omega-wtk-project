#include "omegaWTK/Core/Core.h"
#include "NativeEvent.h"
#include "NativeItem.h"
#include "NativeMenu.h"

#ifndef OMEGAWTK_NATIVE_NATIVEWINDOW_H
#define OMEGAWTK_NATIVE_NATIVEWINDOW_H
namespace OmegaWTK {
    class AppWindow;
}
namespace OmegaWTK::Native {
    INTERFACE NativeWindow {
        friend class ::OmegaWTK::AppWindow;
        INTERFACE_METHOD NativeItemPtr getRootView() ABSTRACT;
    protected:
        OmegaCommon::Vector<NativeItemPtr> windowWidgetRootViews;
        NM menu = nullptr;
    public:
//        void setNativeLayer(NativeLayer *layer){ window_native_layer = layer;};
#ifdef TARGET_MACOS
        bool hasEventEmitter();
    protected:
        NativeEventEmitter *eventEmitter = nullptr;
    public:
#endif
        virtual void enable() = 0;
        virtual void disable() = 0;
        virtual void initialDisplay() = 0;
        // virtual void attachWidgets() = 0;
        virtual void close() = 0;
        void setMenu(NM menu);
        virtual void addNativeItem(NativeItemPtr item) = 0;
        #ifdef TARGET_MACOS
        protected:
        Core::Rect & rect;
        public:
        NativeWindow(Core::Rect & rect);
        #endif
    };
    typedef SharedHandle<NativeWindow> NWH;
    NWH make_native_window(Core::Rect & rect,NativeEventEmitter *emitter);
};

#endif
