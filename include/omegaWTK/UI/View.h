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
    class ScrollView;
    /**
        @brief Controls all the basic functionality of a Widget!
        Sometimes referred to as the CanvasView.
        @relates Widget
     */ 
    class OMEGAWTK_EXPORT View : public Native::NativeEventEmitter {
        Core::Vector<SharedHandle<View>> subviews;
    protected:
        SharedHandle<Composition::ViewRenderTarget> renderTarget;
        friend class Widget;
    private:
        Composition::LayerTree *widgetLayerTree;
        View * parent_ptr;
        Core::Rect rect;
        ViewDelegate *delegate = nullptr;
        virtual bool hasDelegate();
        void addSubView(View *view);
        void removeSubView(View * view);
        friend class AppWindow;
        SharedHandle<Composition::LayerTree::Limb> layerTreeLimb;
        friend class ViewAnimator;
        friend class ScrollView;
        Composition::Compositor *getWidgetCompositor();
    protected:
        /**
            Constructs a View using a Rect param; (With NO Layers!!)
            NOTE:
            This Constructed is only called when making a VideoView
            In other words, the View that is returned has NO layers will be completlty blank.
            @param rect[in] The Rect to use
            @param parent[in] The Parent View
            @returns A View!
         */
        View(const Core::Rect & rect,View *parent);
        /**
            Constructs a View using a Rect param and a NativeItem; (With NO Layers!!)
            NOTE:
            This Constructed is only called when making a ScrollView.
            In other words, the View that is returned has NO layers will be completlty blank.
            @param rect[in] The Rect to use
            @param nativeItem[in] The Native View to bind to
            @param parent[in] The Parent View
            @returns A View!
         */
        View(const Core::Rect & rect,Native::NativeItemPtr nativeItem,View *parent);
    public:
        Core::Rect & getRect(){ return rect;};
        Composition::LayerTree::Limb * getLayerTreeLimb(){ return layerTreeLimb.get();};
        bool isRootView(){return parent_ptr == nullptr;};
        virtual void setDelegate(ViewDelegate *_delegate);
        virtual void resize(Core::Rect newRect);
        /**
            Constructs a View using a Rect param and constructs a LayerTree::Limb to be used on the layerTree;
            @param rect The Rect to use
            @param layerTree
            @returns A View!
         */
        View(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent = nullptr);
        ~View();
    };

    /// CanvasView Def.
    typedef View CanvasView;

    /**
        @brief The Root View delegate class!
     */
    class OMEGAWTK_EXPORT ViewDelegate : public Native::NativeEventProcessor {
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
    
    class ScrollViewDelegate;

    class OMEGAWTK_EXPORT ScrollView : public View {
        SharedHandle<View> child;
        Core::Rect * childViewRect;
        ScrollViewDelegate *delegate = nullptr;
        bool hasDelegate();
        bool hasVericalScrollBar,hasHorizontalScrollBar;
    public:
        void toggleVerticalScrollBar();
        void toggleHorizontalScrollBar();
        void setDelegate(ViewDelegate *_delegate);
        /**
            @param rect The Rect to use
            @returns A ScrollView!
         */
        ScrollView(const Core::Rect & rect,SharedHandle<View> child,bool hasVericalScrollBar,bool hasHorizontalScrollBar,View *parent = nullptr);
    };

    class OMEGAWTK_EXPORT ScrollViewDelegate : public Native::NativeEventProcessor {
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class ScrollView;
    protected:
        ScrollView *scrollView;

        virtual void onScrollLeft(){};
        virtual void onScrollRight(){};
        virtual void onScrollDown(){};
        virtual void onScrollUp(){};
    };

};

#endif
