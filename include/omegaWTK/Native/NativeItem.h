#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEITEM_H
#define OMEGAWTK_NATIVE_NATIVEITEM_H

namespace OmegaWTK {
    namespace Native {
        class NativeEventEmitter;
        class NativeEvent;

        class NativeLayer {
            public:
            virtual void redraw() = 0;
        };
        /// Null Object Placeholder for native object!
        class NativeItem {
            friend void set_native_item_event_emitter(NativeItem * ptr,NativeEventEmitter * emitter);
            NativeEventEmitter *event_emitter;
            protected:
            NativeLayer *parentLayer;
            public:
            void sendEventToEmitter(NativeEvent *ptr);
            bool hasEventEmitter();
            void setParentLayer(NativeLayer *layer){ parentLayer = layer;};
            virtual void *getBinding() = 0;
            virtual void enable() = 0;
            virtual void disable() = 0;
            NativeItem();
            // ~NativeItem();
        };
        typedef NativeItem *NativeItemPtr;
        
        typedef enum : OPT_PARAM {
            Default,
//            PushButton,
//            TextInput
        }ItemType;

        /// Makes Native Item (If `parent` arg is nullptr, the native_item will be attached as a child to the root HWND or NSView *)
        NativeItemPtr make_native_item(Core::Rect rect,Native::ItemType type = Native::Default,NativeItemPtr parent = nullptr);
        void set_native_item_event_emitter(NativeItemPtr ptr,NativeEventEmitter * emitter);
    }
};

#endif
