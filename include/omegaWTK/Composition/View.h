#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"

#ifndef OMEGAWTK_COMPOSITION_VIEW_H
#define OMEGAWTK_COMPOSITION_VIEW_H

namespace OmegaWTK::Composition {
    class ViewDelegate;
    /// The root view class!
    class View : public Layer, Native::NativeEventEmitter {
        Native::NativeItemPtr native;
        ViewDelegate *delegate = nullptr;
        bool hasDelegate();
    public:
        void setDelegate(ViewDelegate *_delegate);
        View(const Core::Rect & rect);
    };
    /// The Root View delegate class!
    class ViewDelegate : public Native::NativeEventResponder {
        View * view;
        friend class View;
        protected:
        virtual void onMouseEnter(Native::NativeEventPtr event) = 0;
        virtual void onMouseExit(Native::NativeEventPtr event) = 0;
        virtual void onLeftMouseDown(Native::NativeEventPtr event) = 0;
        virtual void onLeftMouseUp(Native::NativeEventPtr event) = 0;
        virtual void onRightMouseDown(Native::NativeEventPtr event) = 0;
        virtual void onRightMouseUp(Native::NativeEventPtr event) = 0;
        virtual void onKeyDown(Native::NativeEventPtr event) = 0;
        virtual void onKeyUp(Native::NativeEventPtr event) = 0;
        public:
        ViewDelegate();
        ~ViewDelegate();
    };

};

#endif