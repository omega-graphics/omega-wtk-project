#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"

#ifndef OMEGAWTK_COMPOSITION_VIEW_H
#define OMEGAWTK_COMPOSITION_VIEW_H

namespace OmegaWTK {

    class LayerOwner {
        protected:
        Composition::Layer *layer;
        public:
        /**
            Constructs a Layer using the Rect param provided!
            @param rect The Rect to Use!
            @returns A LayerOwner
        */
        LayerOwner(const Core::Rect & rect,Composition::Layer * layer);
    };

    class ViewDelegate;
    /// The root view class!
    class View : public LayerOwner , Native::NativeEventEmitter {
        Core::Vector<View *> subviews;
        View *parent_ptr = nullptr;
        Core::Rect rect;
        Native::NativeItemPtr native;
        ViewDelegate *delegate = nullptr;
        bool hasDelegate();
    public:
        bool isRootView(){return parent_ptr == nullptr;};
        void setDelegate(ViewDelegate *_delegate);
        void addSubView(View *view);
        void removeSubView(View *view);
        View(const Core::Rect & rect,Composition::Layer *layer_to_use,Native::NativeItemPtr item);
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

    View *make_view(const Core::Rect & rect);

};

#endif