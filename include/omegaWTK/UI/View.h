/**
 @file View.h
 
 */

#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/LayerAnimation.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeEvent.h"
#include "omegaWTK/Native/NativeApp.h"

#ifndef OMEGAWTK_UI_VIEW_H
#define OMEGAWTK_UI_VIEW_H

namespace OmegaWTK {
    class Widget;
    class AppInst;
    class ViewAnimator;
    class ViewDelegate;
    /**
        @brief Controls all the basic functionality of a Widget!
        Sometimes referred to as the CanvasView.
        @relates Widget
     */
OMEGAWTK_EXPORT class View: public Native::NativeEventEmitter {
        Core::Vector<SharedHandle<View>> subviews;
    protected:
        SharedHandle<Composition::ViewRenderTarget> renderTarget;
        friend class Widget;
    private:
        Composition::LayerTree *widgetLayerTree;
        View * parent_ptr;
        Core::Rect rect;
        ViewDelegate *delegate = nullptr;
        bool hasDelegate();
        void addSubView(View *view);
        void removeSubView(View * view);
        friend class AppWindow;
        SharedHandle<Composition::LayerTree::Limb> layerTreeLimb;
        friend class ViewAnimator;
        Composition::Compositor *getWidgetCompositor();
    public:
        Composition::LayerTree::Limb * getLayerTreeLimb(){ return layerTreeLimb.get();};
        bool isRootView(){return parent_ptr == nullptr;};
        void setDelegate(ViewDelegate *_delegate);
        /**
            Constructs a View using a Rect param and constructs a LayerTree::Limb to be used on the layerTree;
            @param rect The Rect to use
            @param layerTree
            @returns A View!
         */
        View(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent = nullptr);
        /**
            Constructs a View using a Rect param; (With NO Layers!!)
            NOTE:
            This Constructed is only called when making a ScrollView or a VideoView
            In other words, the View that is returned has NO layers will be completlty blank.
            @param rect The Rect to use
            @returns A View!
         */
        View(const Core::Rect & rect,View *parent = nullptr);
        ~View();
    };

    /// CanvasView Def.
    typedef View CanvasView;

    /**
        @brief The Root View delegate class!
     */
OMEGAWTK_EXPORT class ViewDelegate : public Native::NativeEventProcessor {
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class View;
        protected:
        View * view;
        /**
            @name Interface Methods
         
         */
        /// @{
//        /**
//            Called when the view has loaded. NOTE: All View Delegates must implement this method!
//        */
//        virtual void viewHasLoaded(Native::NativeEventPtr event) = 0;
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
