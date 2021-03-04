#include "omegaWTK/Core/Core.h"
#include "NativeEvent.h"
#include "NativeItem.h"
#include "NativeMenu.h"

#ifndef OMEGAWTK_NATIVE_NATIVEWINDOW_H
#define OMEGAWTK_NATIVE_NATIVEWINDOW_H

namespace OmegaWTK::Native {
    class NativeWindow {
        friend class AppWindow;
    protected:
        Core::Vector<NativeItemPtr> windowWidgetRootViews;
        NM menu = nullptr;
//        NativeLayerTreeLimb *window_native_layer = nullptr;
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
        virtual void attachWidgets() = 0;
        void setMenu(NM menu);
        void addNativeItem(NativeItemPtr item);
        #ifdef TARGET_MACOS
        protected:
        Core::Rect & rect;
        public:
        NativeWindow(Core::Rect & rect);
        #endif
    };
    typedef NativeWindow *NWH;
    NWH make_native_window(Core::Rect & rect,NativeEventEmitter *emitter);
};

#endif