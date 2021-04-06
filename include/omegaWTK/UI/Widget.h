#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Compositor.h"
#include "View.h"

#ifndef OMEGAWTK_UI_WIDGET_H
#define OMEGAWTK_UI_WIDGET_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;
class VideoView;
class WidgetObserver;

/**
 A moduler UI component. 
 Can be attached either directly to an AppWindow or as a child of another Widget.
 @see AppWindow
*/
class OMEGAWTK_EXPORT  Widget {
protected:
    SharedHandle<CanvasView> rootView;
    SharedHandle<Widget> parent;
    SharedHandle<Composition::LayerTree> layerTree;
    /**
     Constructs a Layer with Widget's Compositor Pointer and Core::Rect
     */
    SharedHandle<Composition::Layer> makeLayer(const Core::Rect & rect);
    Composition::Compositor * compositor;
        /**
     Makes a Canvas View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A standard View
     */
    SharedHandle<CanvasView> makeCanvasView(const Core::Rect & rect,View *parent);
    /**
     Makes a Video View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A Video View
     */
//    SharedHandle<VideoView> makeVideoView(const Core::Rect & rect,View *parent);
    
private:
    bool resizeable;
    /// Observers
    Core::Vector<WidgetObserver *> observers;
protected:
    typedef enum : OPT_PARAM {
        Resize,
        Show,
        Hide
    } WidgetEventType;
    void notifyObservers(WidgetEventType eventType,Core::Rect * rect);
private:
    friend class AppWindow;
    friend class AppWindowManager;
public:
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
    virtual void resize(Core::Rect & newRect) = 0;
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
    void refresh();
    Widget(const Core::Rect & rect,SharedHandle<Widget> parent);
//    Widget(Widget &widget);
    ~Widget();
};

#define WIDGET_NOTIFY_OBSERVERS_SHOW() notifyObservers(Widget::Show,nullptr)
#define WIDGET_NOTIFY_OBSERVERS_HIDE() notifyObservers(Widget::Hide,nullptr)
#define WIDGET_NOTIFY_OBSERVERS_RESIZE(rect) notifyObservers(Widget::Resize,&rect)


/// Similar to a Widget Delegate but a Widget can have more than one.
class OMEGAWTK_EXPORT  WidgetObserver {
    friend class Widget;
    bool hasAssignment;
protected:
    Widget *widget;
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

};

#endif
