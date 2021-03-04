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
    class ViewDelegate;
    /**
        A Global View class that controls all the basic functions of a Widget!
        @relates Widget
        
     */
    class OMEGAWTK_EXPORT View: public Native::NativeEventEmitter {
        Core::Vector<SharedHandle<View>> subviews;
        UniqueHandle<Composition::ViewRenderTarget> renderTarget;
        Composition::LayerTree *widgetLayerTree;
        View * parent_ptr;
        Core::Rect rect;
        ViewDelegate *delegate = nullptr;
        bool hasDelegate();
        void addSubView(View *view);
        void removeSubView(View * view);
        friend class Widget;
        friend class AppWindow;
        SharedHandle<Composition::LayerTree::Limb> layerTreeLimb;
    public:
        Composition::LayerTree::Limb * getLayerTreeLimb(){ return layerTreeLimb.get();};
        bool isRootView(){return parent_ptr == nullptr;};
        void setDelegate(ViewDelegate *_delegate);
        /**
            Constructs a View using a Rect param;
            @param rect The Rect to use
            @returns A View!
         */
        View(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent = nullptr);
    };
    /// The Root View delegate class!
    class OMEGAWTK_EXPORT ViewDelegate : public Native::NativeEventProcessor {
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class View;
        protected:
        View * view;
        /**
            @name Interface Methods
         
         */
        /// @{
        /**
            Called when the Mouse Enters the View
         */
        virtual void onMouseEnter(Native::NativeEventPtr event) {};
        /**
            Called when the Mouse Exits the View
         */
        virtual void onMouseExit(Native::NativeEventPtr event) {};
        /**
            Called when the Left Mouse Button is pressed
         */
        virtual void onLeftMouseDown(Native::NativeEventPtr event) {};
        /**
            Called when the Left Mouse Button is raised after being pressed
         */
        virtual void onLeftMouseUp(Native::NativeEventPtr event) {};
        /**
            Called when the Right Mouse Button is pressed
         */
        virtual void onRightMouseDown(Native::NativeEventPtr event) {};
        /**
            Called when the Right Mouse Button is raised after being pressed
         */
        virtual void onRightMouseUp(Native::NativeEventPtr event) {};
        /**
            Called when a key on a keyboard is pressed
         */
        virtual void onKeyDown(Native::NativeEventPtr event) {};
        /**
            Called when a key on a keyboard is raised after being pressed
         */
        virtual void onKeyUp(Native::NativeEventPtr event) {};
        /// @}
        public:
        ViewDelegate();
        ~ViewDelegate();
    };
};

#endif
