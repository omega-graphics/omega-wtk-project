

%module OmegaWTK
%{
    #include "../include/OmegaWTK.h"
%}
// %include <swiginterface.i>
// %interface_impl(OmegaWTK::WidgetObserver);
// %interface_impl(OmegaWTK::ViewDelegate);

namespace OmegaWTK {
    namespace Core {
        struct Rect {};
    }
    namespace Native {
        struct NativeEvent {

        };
        typedef NativeEvent *NativeEventPtr;
    };
    namespace Composition {
        class Layer {

        };
        class LayerTree {
        public:
            // struct Limb {

            // };
        };
    }
    class ViewDelegate {
    protected:
         virtual void onMouseEnter(Native::NativeEventPtr event);
        /**
            Called when the Mouse Exits the View
         */
        virtual void onMouseExit(Native::NativeEventPtr event);
        /**
            Called when the Left Mouse Button is pressed
         */
        virtual void onLeftMouseDown(Native::NativeEventPtr event);
        /**
            Called when the Left Mouse Button is raised after being pressed
         */
        virtual void onLeftMouseUp(Native::NativeEventPtr event);
        /**
            Called when the Right Mouse Button is pressed
         */
        virtual void onRightMouseDown(Native::NativeEventPtr event);
        /**
            Called when the Right Mouse Button is raised after being pressed
         */
        virtual void onRightMouseUp(Native::NativeEventPtr event);
        /**
            Called when a key on a keyboard is pressed
         */
        virtual void onKeyDown(Native::NativeEventPtr event);
        /**
            Called when a key on a keyboard is raised after being pressed
         */
        virtual void onKeyUp(Native::NativeEventPtr event);
    };
    class View {
    public:
        Core::Rect & getRect();
        Composition::LayerTree::Limb * getLayerTreeLimb();
        bool isRootView();
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


    class WidgetObserver {
    public:
        WidgetObserver();
        /// Implement in subclasses!
        /// Called when the Widget has changed size.
        virtual void onWidgetChangeSize(Core::Rect oldRect,Core::Rect & newRect){};
        /// Called when the Widget has just been Hidden.
        virtual void onWidgetDidHide(){};
        /// Called when the Widget has just been Shown.
        virtual void onWidgetDidShow(){};
    };
    class Widget {
    public:
        Core::Rect & rect();
        void setParentView(View *view);
        /**
        Add a WidgetObserver to be notified.
        */
        void addObserver(WidgetObserver * observer);
        /**
        Remove a WidgetObserver from the list of observers currently listening.
        */
        void removeObserver(WidgetObserver *observerPtr);
        bool & isResizable();
        virtual void resize(Core::Rect & newRect);
        /**
        Show the Widget if hidden.
        */
        void show();
        /**
        Hide the Widget if shown
        */
        void hide();
        /**
        Redraws the Widget with the current state of its layer tree.
        */
        virtual void refresh();

        Widget(Widget &widget);
        ~Widget();
    };
}