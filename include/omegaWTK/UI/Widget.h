
#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeTheme.h"
#include "omegaWTK/Composition/Canvas.h"

#ifndef OMEGAWTK_UI_WIDGET_H
#define OMEGAWTK_UI_WIDGET_H

namespace OmegaWTK {

namespace Composition {
    class LayerTree;
}

class AppWindow;
class AppWindowManager;

class View;
typedef View CanvasView;
OMEGACOMMON_SHARED_CLASS(CanvasView);
class VideoView;
OMEGACOMMON_SHARED_CLASS(VideoView);
class SVGView;
OMEGACOMMON_SHARED_CLASS(SVGView);
class UIView;
OMEGACOMMON_SHARED_CLASS(UIView);

typedef Composition::CanvasElementTag UIViewTag;

class WidgetObserver;
class WidgetTreeHost;
class Widget;
OMEGACOMMON_SHARED_CLASS(Widget);
OMEGACOMMON_SHARED_CLASS(WidgetObserver);


/**
 @brief A moduler UI component. 
 Can be attached to a WidgetTreeHost or another Widget as a child.
 @paragraph

 @see AppWindow
*/
class OMEGAWTK_EXPORT  Widget : public Native::NativeThemeObserver {
    bool initialDrawComplete = false;

    void onThemeSetRecurse(Native::ThemeDesc &desc);

    using Native::NativeThemeObserver::onThemeSet;
protected:

    SharedHandle<CanvasView> rootView;
    WidgetPtr parent;
    SharedHandle<Composition::LayerTree> layerTree;
    /**
     The WidgetTreeHost that hosts this widget.
    */
    WidgetTreeHost *treeHost = nullptr;
        /**
     Makes a Canvas View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A standard View
     */
    CanvasViewPtr makeCanvasView(const Core::Rect & rect,View *parent);

    //    /**
    //  Makes a Canvas View attached to this widget and returns it.
    //  @param rect The Rectangle to use
    //  @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
    //  @returns A standard View
    //  */
    // TextViewPtr makeTextView(const Core::Rect & rect,View *parent);

    /**
     Makes an SVG View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A Video View
     */
    SVGViewPtr makeSVGView(const Core::Rect & rect,View *parent);

    /**
     Makes a Video View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A Video View
     */
    VideoViewPtr makeVideoView(const Core::Rect & rect,View *parent);

     /**
     Makes a UI View attached to this widget and returns it.
     @param rect The Rectangle to use
     @param parent The Parent View (NOTE: This view MUST be within this widget's view heirarchy)
     @returns A Video View
     */
    UIViewPtr makeUIView(const Core::Rect & rect,View *parent,UIViewTag tag = "");
    
private:
    OmegaCommon::Vector<WidgetPtr> children;
    void setTreeHostRecurse(WidgetTreeHost *host);
    void removeChildWidget(WidgetPtr ptr);
    /// Observers
    OmegaCommon::Vector<WidgetObserverPtr> observers;
protected:
    typedef enum : OPT_PARAM {
        Resize,
        Show,
        Hide,
        Detach,
        Attach
    } WidgetEventType;
    void notifyObservers(WidgetEventType eventType,void* params);
    /**
    @brief Initial render of the Widget
    @note MUST be implemented by all Widgets*/
    INTERFACE_METHOD void init() ABSTRACT;
private:
    friend class AppWindow;
    friend class AppWindowManager;
    friend class WidgetTreeHost;
public:
    OMEGACOMMON_CLASS("OmegaWTK.Widget")
    /**
     Get the Widget's root View's rect
    */
    Core::Rect & rect();
    void setParentWidget(Widget *widget);
    /**
     Add a WidgetObserver to be notified.
    */
    void addObserver(WidgetObserverPtr observer);
    /**
     Remove a WidgetObserver from the list of observers currently listening.
     @note RARELY USED
    */
    void removeObserver(WidgetObserverPtr observerPtr);
    // bool & isResizable();
    void resize(Core::Rect & newRect){
        // std::cout << "THIS WIDGET IS NOT RESIZABLE" << std::endl;
    };
    /**
     Show the Widget if hidden.
    */
    void show();
    /**
     Hide the Widget if shown
    */
    void hide();
protected:
    Widget(const Core::Rect & rect,Widget * parent);
public:
    ~Widget() override FALLTHROUGH;
};

// #define WIDGET_TEMPLATE_BEGIN()
// #define WIDGET_TEMPLATE_VIEW(class_name,...)
// #define WIDGET_TEMPLATE_END()


#define WIDGET_NOTIFY_OBSERVERS_SHOW() notifyObservers(Widget::Show,nullptr)
#define WIDGET_NOTIFY_OBSERVERS_HIDE() notifyObservers(Widget::Hide,nullptr)
#define WIDGET_NOTIFY_OBSERVERS_RESIZE(rect) notifyObservers(Widget::Resize,&(rect))


/** 
 @brief Similar to the concept of a Widget Delegate but a Widget can have more than one.
 @paragraph
*/
class OMEGAWTK_EXPORT  WidgetObserver {
    friend class Widget;
    bool hasAssignment;
protected:
    Widget *widget;
public:
    WidgetObserver();
    /// Implement in subclasses!
    /// Called when the Widget has been attached to a WidgetTree.
    INTERFACE_METHOD void onWidgetAttach(Widget *parent) DEFAULT;
     /// Called when the Widget has been dettached from a WidgetTree.
    INTERFACE_METHOD void onWidgetDetach(Widget *parent) DEFAULT;
    /// Called when the Widget has changed size.
    INTERFACE_METHOD void onWidgetChangeSize(Core::Rect oldRect,Core::Rect & newRect) DEFAULT;
    /// Called when the Widget has just been Hidden.
    INTERFACE_METHOD void onWidgetDidHide() DEFAULT;
    /// Called when the Widget has just been Shown.
    INTERFACE_METHOD void onWidgetDidShow() DEFAULT;

    INTERFACE_METHOD ~WidgetObserver() FALLTHROUGH;
};

template<class Ty>
class WidgetState;


template<class State_Ty>
class WidgetStateObserver;

template<class Ty>
class WidgetStateObserver<WidgetState<Ty>> {
protected:
    INTERFACE_METHOD void stateHasChanged(Ty & newVal) DEFAULT;
};


template<class Ty>
class OMEGAWTK_EXPORT WidgetState {
    Core::Optional<Ty> val;
    OmegaCommon::Vector<WidgetStateObserver<WidgetState<Ty>> *> observers;
public: 
    void setValue(Ty newVal){
        val = newVal;
        for(auto & observer : observers){
            observer->stateHasChanged(newVal);
        };
    };
    static SharedHandle<WidgetState<Ty>> Create(Core::Optional<Ty> initalValue = {}){
        SharedHandle<WidgetState<Ty>> rc(new WidgetState<Ty>);
        rc->val = initalValue;
        return rc;
    };

    void addObserver(SharedHandle<WidgetStateObserver<WidgetState<Ty>>> observer){
        observers.push_back(observer.get());
    };

    void addObserver(WidgetStateObserver<WidgetState<Ty>> *observer){
        observers.push_back(observer);
    };

    void removeObserver(WidgetStateObserver<WidgetState<Ty>> *observer){
        auto ob_it = observers.begin();
        while(ob_it != observers.end()){
            if(*ob_it == observer){
                observers.erase(ob_it);
                break;
            };
            ++ob_it;
        };
    };
};

/**
 * @brief A single view widget responsible for managiing one view's capability.
 * 
 */
 template<class ViewT>
class OMEGAWTK_EXPORT WrapperWidget {
public:
    static SharedHandle<WrapperWidget<ViewT>> Create(const Core::Rect & rect,WidgetPtr parent);
    SharedHandle<ViewT> getUnderlyingView();
};

};

#endif
