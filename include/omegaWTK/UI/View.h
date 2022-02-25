/**
 @file View.h
 
 */

#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeApp.h"

#ifndef OMEGAWTK_UI_VIEW_H
#define OMEGAWTK_UI_VIEW_H

namespace OmegaWTK {
    namespace Composition {
        class ViewAnimator;
        class Font;
        class TextRect;
    }

    namespace Native {
        class NativeEvent;
        typedef SharedHandle<NativeEvent> NativeEventPtr;
    }

    
    class Widget;
    class AppInst;
    class ViewDelegate;
    class ScrollView;

    /**
        @brief Controls all the basic functionality of a Widget!
        Sometimes referred to as the CanvasView.
        @relates Widget
     */ 
    class OMEGAWTK_EXPORT View : public Native::NativeEventEmitter {
        OmegaCommon::Vector<SharedHandle<View>> subviews;
    protected:
        SharedHandle<Composition::ViewRenderTarget> renderTarget;
        friend class Widget;
    private:
        Composition::CompositorClientProxy proxy;
        void setFrontendRecurse(Composition::Compositor *frontend);
        Composition::LayerTree *widgetLayerTree;
        View * parent_ptr;
        Core::Rect rect;
        ViewDelegate *delegate = nullptr;
        virtual bool hasDelegate();
        void addSubView(View *view);
        void removeSubView(View * view);
        friend class AppWindow;
        SharedHandle<Composition::LayerTree::Limb> layerTreeLimb;
        friend class Composition::ViewAnimator;
        friend class ScrollView;
    protected:
//        /**
//            Constructs a View using a Rect param; (With NO Layers!!)
//            NOTE:
//            This Constructed is only called when making a VideoView
//            In other words, the View that is returned has NO layers will be completlty blank.
//            @param rect[in] The Rect to use
//            @param parent[in] The Parent View
//            @returns A View!
//         */
//        View(const Core::Rect & rect,View *parent);
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
        View(const Core::Rect & rect,Native::NativeItemPtr nativeItem,Composition::LayerTree *layerTree,View *parent);
        /**
            Constructs a View using a Rect param and constructs a LayerTree::Limb to be used on the layerTree;
            @param rect The Rect to use
            @param layerTree
            @returns A View!
         */
        View(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent = nullptr);
    public:
        OMEGACOMMON_CLASS("OmegaWTK.View")
        /**
         * @brief Create A Layer
         * @param rect The Rectangle defining the bounds of the layer.
         * @returns Layer*/
        SharedHandle<Composition::Layer> makeLayer(Core::Rect rect);

        /**
         * @brief Create a Canvas that renders to CanvasFrames compatible with a Layer.
         * @param targetLayer The Layer to target.
         * @returns Canvas*/
        SharedHandle<Composition::Canvas> makeCanvas(SharedHandle<Composition::Layer> & targetLayer);

        /// @brief Retrieves the Rect that defines the position and bounds of the View.
        Core::Rect & getRect(){ return rect;};
        /// @brief Retrieves the corresponding limb bound to this View on the parent's Widget's LayerTree.
        SharedHandle<Composition::LayerTree::Limb> & getLayerTreeLimb(){ return layerTreeLimb;};
        /// @brief Checks to see if this View is the root View of a Widget.
        bool isRootView(){return parent_ptr == nullptr;};

        /// @brief Sets the object to recieve View related events.
        virtual void setDelegate(ViewDelegate *_delegate);

        /// @brief Resize the view synchronously.
        /// @note If you wish to animate the View resize, please use the ViewAnimator to perform that action.
        virtual void resize(Core::Rect newRect);

        /// @brief Starts a Composition Session for this View.
        /// @paragraph Upon invocation, this will allow Canvases to render to child Layers in the corresponding LayerTree::Limb
        /// and it will allow submission of render and animation commands from the child LayerAnimators and ViewAnimator.
        /// If one attempts to try animate or render to the View or any child Layers without calling this method FIRST, will recieve an access error.
        void startCompositionSession();

        /// @brief Ends a Composition Session for this View.
        /// @paragraph This method closes the submission queue of all render commands and submits them to the Compositor.
        /// Any commands posted to the CompositorClientProxy after invocation of this method will be ignored and an access error will be thrown.
        void endCompositionSession();

        /// @brief Make the View visible.
        void enable();

        /// @brief Make the View invisible.
        void disable();
        ~View();
    };

    /// CanvasView Def.
    typedef View CanvasView;

    /**
        @brief The Root View delegate class!
     */
    INTERFACE OMEGAWTK_EXPORT ViewDelegate : public Native::NativeEventProcessor {
        void onRecieveEvent(Native::NativeEventPtr event);
        ViewDelegate *forwardDelegate = nullptr;
        friend class View;
        protected:
        View * view;

        void setForwardDelegate(ViewDelegate *delegate);
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
        virtual void onMouseEnter(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when the Mouse Exits the View
         */
        virtual void onMouseExit(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when the Left Mouse Button is pressed
         */
        virtual void onLeftMouseDown(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when the Left Mouse Button is raised after being pressed
         */
        virtual void onLeftMouseUp(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when the Right Mouse Button is pressed
         */
        virtual void onRightMouseDown(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when the Right Mouse Button is raised after being pressed
         */
        virtual void onRightMouseUp(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when a key on a keyboard is pressed
         */
        virtual void onKeyDown(Native::NativeEventPtr event) DEFAULT;
        /**
            Called when a key on a keyboard is raised after being pressed
         */
        virtual void onKeyUp(Native::NativeEventPtr event) DEFAULT;
        /// @}
        public:
        ViewDelegate();
        ~ViewDelegate();
    };

    // class ViewTreeChildObserver {
    //     SharedHandle<View> view;
    // public:
        
    // };

    
    class ScrollViewDelegate;

    /// @brief ScrollView
    class OMEGAWTK_EXPORT ScrollView : public View {
        SharedHandle<View> child;
        Core::Rect * childViewRect;
        ScrollViewDelegate *delegate = nullptr;
        bool hasDelegate();
        bool hasVerticalScrollBar,hasHorizontalScrollBar;
        friend class Widget;
        explicit ScrollView(const Core::Rect & rect, SharedHandle<View> child, bool hasVerticalScrollBar, bool hasHorizontalScrollBar, Composition::LayerTree *layerTree, View *parent = nullptr);
    public:
        void toggleVerticalScrollBar();
        void toggleHorizontalScrollBar();
        void setDelegate(ScrollViewDelegate *_delegate);
        /**
            @param rect The Rect to use
            @returns A ScrollView!
         */
       
    };

    class OMEGAWTK_EXPORT ScrollViewDelegate : public Native::NativeEventProcessor {
        void onRecieveEvent(Native::NativeEventPtr event);
        friend class ScrollView;
    protected:
        ScrollView *scrollView;

        INTERFACE_METHOD void onScrollLeft() DEFAULT;
        INTERFACE_METHOD void onScrollRight() DEFAULT;
        INTERFACE_METHOD void onScrollDown() DEFAULT;
        INTERFACE_METHOD void onScrollUp() DEFAULT;
    };

    class OMEGAWTK_EXPORT ClickableViewHandler : public ViewDelegate {
        std::function<void()> hover_begin_handler,hover_end_handler,click_handler,press_handler,release_handler;
        void onMouseEnter(Native::NativeEventPtr event) override;
        void onMouseExit(Native::NativeEventPtr event) override;
        void onLeftMouseDown(Native::NativeEventPtr event) override;
        void onLeftMouseUp(Native::NativeEventPtr event) override;
    public:
        static SharedHandle<ClickableViewHandler> Create();
        void onHoverBegin(std::function<void()> handler);
        void onHoverEnd(std::function<void()> handler);
        void onPress(std::function<void()> handler);
        void onRelease(std::function<void()> handler);
        void onClick(std::function<void()> handler);
    };


    /// @brief TextView
    class OMEGAWTK_EXPORT TextView : public View {

        SharedHandle<Composition::TextRect> textRect;

        SharedHandle<Composition::Font> font;

        SharedHandle<Composition::Canvas> rootLayerCanvas;

        UniqueHandle<ClickableViewHandler> clickableHandler;

         SharedHandle<Composition::Layer> cursorLayer;

        SharedHandle<Composition::Canvas> cursorCanvas;

        OmegaWTK::UniString str;
        bool editMode = false;
        void moveTextCursorToMousePoint(Core::Position & pos);
        void enableCursor();
        void disableCursor();
        void pushChar(Unicode32Char & ch);
        void popChar();
        void commitChanges();
        friend class TextViewDelegate;
        friend class Widget;
        explicit TextView(const Core::Rect & rect,Composition::LayerTree * layerTree,View * parent,bool io);
    public:
        void updateFont(SharedHandle<Composition::Font> & font);
        void setContent(const UChar * str);
    };

    class OMEGAWTK_EXPORT TextViewDelegate : public ViewDelegate { 

        UniqueHandle<ClickableViewHandler> clickHandler;

        void onKeyDown(Native::NativeEventPtr event) override;
        void onKeyUp(Native::NativeEventPtr event) override;
    public:
        TextViewDelegate(TextView *view);
        void toggleEdit();
        bool editMode();
        OmegaWTK::UniString & getString();
    };

};

#endif
