#include "omegaWTK/Core/Core.h"
#include "View.h"

#ifndef OMEGAWTK_UI_WIDGET_H
#define OMEGAWTK_UI_WIDGET_H

namespace OmegaWTK {

class AppWindow;
class AppWindowManager;
class VideoView;
class WidgetObserver;
class WidgetTreeHost;

/**
 @brief A moduler UI component. 
 Can be attached to a WidgetTreeHost or another Widget as a child.
 @paragraph

 @see AppWindow
*/
class OMEGAWTK_EXPORT  Widget {
protected:
    SharedHandle<CanvasView> rootView;
    SharedHandle<Widget> parent;
    SharedHandle<Composition::LayerTree> layerTree;
    /**
     The WidgetTreeHost that hosts this widget.
    */
    WidgetTreeHost *treeHost;
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
    /// Observers
    OmegaCommon::Vector<WidgetObserver *> observers;
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
    friend class WidgetTreeHost;
public:
    /**
     Get the Widget's root View's rect
    */
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
    /**
     Redraws the Widget with the current state of its layer tree.
     */
    virtual void refresh();
protected:
    Widget(const Core::Rect & rect,WidgetTreeHost *treeHost,SharedHandle<Widget> parent);
public:
    virtual ~Widget();
};

#define WIDGET_TEMPLATE_BEGIN()
#define WIDGET_TEMPLATE_VIEW(class_name,args)
#define WIDGET_TEMPLATE_END()

#define WIDGET_CONSTRUCTOR_DEFAULT(class_name) class_name(const Core::Rect & rect,WidgetTreeHost *parentHost,SharedHandle<Widget> parent)
#define WIDGET_CONSTRUCTOR(class_name,params...) class_name(const Core::Rect & rect,WidgetTreeHost *parentHost,SharedHandle<Widget> parent,params)
#define WIDGET_CONSTRUCT_SUPER() Widget(rect,parentHost,parent)

#define WIDGET_NOTIFY_OBSERVERS_SHOW() notifyObservers(Widget::Show,nullptr)
#define WIDGET_NOTIFY_OBSERVERS_HIDE() notifyObservers(Widget::Hide,nullptr)
#define WIDGET_NOTIFY_OBSERVERS_RESIZE(rect) notifyObservers(Widget::Resize,&rect)


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
    /// Called when the Widget has changed size.
    virtual void onWidgetChangeSize(Core::Rect oldRect,Core::Rect & newRect){};
    /// Called when the Widget has just been Hidden.
    virtual void onWidgetDidHide(){};
    /// Called when the Widget has just been Shown.
    virtual void onWidgetDidShow(){};
};

template<class _Ty>
class WidgetState;


template<class _State_Ty>
class WidgetStateObserver;

template<class _Ty>
class WidgetStateObserver<WidgetState<_Ty>> {
protected:
    virtual void stateHasChanged(_Ty & newVal){
        /// TO BE IMPLEMENTED BY SUBCLASSES!!
    };
};


template<class _Ty>
class WidgetState {
    Core::Optional<_Ty> val;
    OmegaCommon::Vector<WidgetStateObserver<WidgetState<_Ty>> *> observers;
public: 
    void setValue(_Ty newVal){
        val = newVal;
        for(auto & observer : observers){
            observer->stateHasChanged(newVal);
        };
    };
    static SharedHandle<WidgetState<_Ty>> Create(Core::Optional<_Ty> initalValue = {}){
        SharedHandle<WidgetState<_Ty>> rc(new WidgetState<_Ty>);
        rc->val = initalValue;
        return rc;
    };

    void addObserver(SharedHandle<WidgetStateObserver<WidgetState<_Ty>>> observer){
        observers.push_back(observer.get());
    };

    void addObserver(WidgetStateObserver<WidgetState<_Ty>> *observer){
        observers.push_back(observer);
    };

    void removeObserver(WidgetStateObserver<WidgetState<_Ty>> *observer){
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

};

#endif
