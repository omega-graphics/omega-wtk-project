/**
 @file View.h
 
 */

#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"
#include "omegaWTK/Native/NativeApp.h"

#ifndef OMEGAWTK_UI_VIEW_H
#define OMEGAWTK_UI_VIEW_H

namespace OmegaWTK {
    class Widget;
    class AppInst;
    /**
        A class that owns a layer.
        NOTE: This is only implemented by a few classes including the View, and the Window.
        @relates View
     */
    class OMEGAWTK_EXPORT LayerOwner {
        protected:
        Composition::Layer *layer;
        public:
        /**
            Constructs a Layer using the Rect param provided!
            @param rect The Rect to Use!
            @param layer The Later to take control of
            @returns A LayerOwner
        */
        LayerOwner(const Core::Rect & rect,Composition::Layer * layer);
        Composition::Layer *getLayer(){ return layer;};
    };
    class ViewDelegate;
    /**
        A Global View class that controls all the basic functions of a Widget!
        @relates Widget
        
     */
    class OMEGAWTK_EXPORT View : public LayerOwner , Native::NativeEventEmitter {
        Core::Vector<View *> subviews;
        View *parent_ptr = nullptr;
        Core::Rect rect;
        Native::NativeItemPtr native;
        ViewDelegate *delegate = nullptr;
        bool hasDelegate();
        friend class Widget;
        friend void internal_set_root(View *view,Native::NAP ptr);
    public:
        bool isRootView(){return parent_ptr == nullptr;};
        void setDelegate(ViewDelegate *_delegate);
        void addSubView(View *view);
        void removeSubView(View *view);
        /**
            Constructs a View using a Rect param, a ptr to a Layer, and a ptr to a NativeItem;
            @param rect The Rect to use
            @param layer_to_use The Composition Layer to use
            @param item The NativeItemPtr to use
            @returns A View!
         */
        View(const Core::Rect & rect,Composition::Layer *layer_to_use,Native::NativeItemPtr item);
    };
    /// The Root View delegate class!
    class OMEGAWTK_EXPORT ViewDelegate : public Native::NativeEventProcessor {
        View * view;
        friend class View;
        protected:
        /**
            @name Interface Methods
         
         */
        /// @{
        /**
            Called when the Mouse Enters the View
         */
        virtual void onMouseEnter(Native::NativeEventPtr event) = 0;
        /**
            Called when the Mouse Exits the View
         */
        virtual void onMouseExit(Native::NativeEventPtr event) = 0;
        /**
            Called when the Left Mouse Button is pressed
         */
        virtual void onLeftMouseDown(Native::NativeEventPtr event) = 0;
        /**
            Called when the Left Mouse Button is raised after being pressed
         */
        virtual void onLeftMouseUp(Native::NativeEventPtr event) = 0;
        /**
            Called when the Right Mouse Button is pressed
         */
        virtual void onRightMouseDown(Native::NativeEventPtr event) = 0;
        /**
            Called when the Right Mouse Button is raised after being pressed
         */
        virtual void onRightMouseUp(Native::NativeEventPtr event) = 0;
        /**
            Called when a key on a keyboard is pressed
         */
        virtual void onKeyDown(Native::NativeEventPtr event) = 0;
        /**
            Called when a key on a keyboard is raised after being pressed
         */
        virtual void onKeyUp(Native::NativeEventPtr event) = 0;
        /// @}
        public:
        ViewDelegate();
        ~ViewDelegate();
    };
    View *make_view(const Core::Rect & rect);

};

#endif
