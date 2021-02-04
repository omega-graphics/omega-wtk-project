#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEITEM_H
#define OMEGAWTK_NATIVE_NATIVEITEM_H

namespace OmegaWTK {
    namespace Native {
        class NativeEventEmitter;
        class NativeEvent;
        /// Null Object Placeholder for native object!
        class NativeItem {
            friend void set_native_item_event_emitter(NativeItem * ptr,NativeEventEmitter * emitter);
            NativeEventEmitter *event_emitter;
            public:
            void sendEventToEmitter(NativeEvent *ptr);
            bool hasEventEmitter();
            virtual void *getBinding() = 0;
            NativeItem();
        };
        typedef NativeItem *NativeItemPtr;
        
        typedef enum : OPT_PARAM {
            Default,
//            PushButton,
//            TextInput
        }ItemType;

        NativeItemPtr make_native_item(Core::Rect rect,Native::ItemType type = Native::Default);
        void set_native_item_event_emitter(NativeItemPtr ptr,NativeEventEmitter * emitter);
    }
};

#endif
