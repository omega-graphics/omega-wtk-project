#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_NATIVE_NATIVEITEM_H
#define OMEGAWTK_NATIVE_NATIVEITEM_H

namespace OmegaWTK {
    namespace Native {
        class NativeEventEmitter;
        class NativeEvent;

        INTERFACE NativeLayerTreeLimb {
            public:
            // INTERFACE_METHOD(void,commit)
        };
        /// Null Object Placeholder for native object!
        INTERFACE NativeItem {
            friend void set_native_item_event_emitter(NativeItem * ptr,NativeEventEmitter * emitter);
            protected:
            NativeLayerTreeLimb *layerTreelimb;
             NativeEventEmitter *event_emitter;
            public:
            INTERFACE_METHOD void addChildNativeItem(NativeItem *nativeItem) ABSTRACT;

            INTERFACE_METHOD void removeChildNativeItem(NativeItem *nativeItem) ABSTRACT;

            void sendEventToEmitter(NativeEvent *ptr);
            bool hasEventEmitter();
            void setLayerTreeLimb(NativeLayerTreeLimb *limb){ layerTreelimb = limb;};

            INTERFACE_METHOD void resize(Core::Rect & newRect)ABSTRACT;
            INTERFACE_METHOD void * getBinding() ABSTRACT;

            INTERFACE_METHOD void enable()ABSTRACT;
            INTERFACE_METHOD void disable()ABSTRACT;

            /// @name ScrollItem Methods
            ///@{
            virtual void toggleHorizontalScrollBar(bool & state) = 0;
            virtual void toggleVerticalScrollBar(bool & state) = 0;
            /**
             This Method may only be invoked if NativeItem is a ScrollView
            */
            virtual void setClippedView(NativeItem * clippedView) = 0;
            /// @}
            NativeItem();
            // ~NativeItem();
        };
        typedef NativeItem *NativeItemPtr;
        
        typedef enum : OPT_PARAM {
            Default,
            ScrollItem
        }ItemType;

        /// Makes Native Item (If `parent` arg is nullptr, the native_item will be attached as a child to the root HWND or NSView *)
        NativeItemPtr make_native_item(Core::Rect rect,Native::ItemType type = Native::Default,NativeItemPtr parent = nullptr);
        void set_native_item_event_emitter(NativeItemPtr ptr,NativeEventEmitter * emitter);
    }
};

#endif
