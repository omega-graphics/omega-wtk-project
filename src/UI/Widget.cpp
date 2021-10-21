#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/UI/VideoView.h"
#include "omegaWTK/UI/WidgetTreeHost.h"
#include "omegaWTK/UI/SVGView.h"



namespace OmegaWTK {


Widget::Widget(const Core::Rect & rect,Widget * parent):parent(parent){
    parent->children.push_back(this);
    layerTree = std::make_shared<Composition::LayerTree>();
    rootView = SharedHandle<CanvasView>(new CanvasView(rect,layerTree.get(),nullptr));
    // std::cout << "Constructing View for Widget" << std::endl;
    if(parent)
        parent->rootView->addSubView(this->rootView.get());
//    std::cout << "RenderTargetPtr:" << rootView->renderTarget.get() << std::endl;
};

//Widget::Widget(Widget & widget):parent(std::move(widget.parent)),compositor(std::move(widget.compositor)),rootView(std::move(widget.rootView)){
//    
//};

SharedHandle<View> Widget::makeCanvasView(const Core::Rect & rect,View *parent){
    return SharedHandle<CanvasView>(new CanvasView(rect,layerTree.get(),parent));
};

SharedHandle<SVGView> Widget::makeSVGView(const Core::Rect & rect,View *parent){
    return SharedHandle<SVGView>(new SVGView(rect,layerTree.get(),parent));
}

SharedHandle<VideoView> Widget::makeVideoView(const Core::Rect & rect,View * parent){
    return SharedHandle<VideoView>(new VideoView(rect,layerTree.get(),parent));
};

Core::Rect & Widget::rect(){
    return rootView->getRect();
};

void Widget::show(){

    rootView->renderTarget->getNativePtr()->enable();
    WIDGET_NOTIFY_OBSERVERS_SHOW();

};
void Widget::hide(){
    rootView->renderTarget->getNativePtr()->disable();
    WIDGET_NOTIFY_OBSERVERS_HIDE();
};

void Widget::addObserver(WidgetObserver * observer){
    if(!observer->hasAssignment) {
        observers.push_back(observer);
        observer->hasAssignment = true;
    };
};

void Widget::setTreeHostRecurse(WidgetTreeHost *host){
    treeHost = host;
    rootView->setFrontendRecurse(host->compPtr());
    for(auto c : children){
        c->setTreeHostRecurse(host);
    };
};

void Widget::removeObserver(WidgetObserver *observerPtr){
    auto it = observers.begin();
    while(it != observers.end()){
        if(*it == observerPtr){
            observers.erase(it);
            observerPtr->hasAssignment = false;
            break;
        };
        ++it;
    };
};

void Widget::notifyObservers(Widget::WidgetEventType event_ty,void* params){
    for(auto & observer : observers){
        switch (event_ty) {
            case Show : {
                observer->onWidgetDidShow();
                break;
            };
            case Hide : {
                observer->onWidgetDidHide();
                break;
            };
            case Resize : {
                observer->onWidgetChangeSize(*(Core::Rect *)params,rootView->rect);
                break;
            }
            case Attach : {
                observer->onWidgetAttach((Widget *)params);
                break;
            }
            case Detach : {
                observer->onWidgetDetach((Widget *)params);
                break;
            }
        }
    };
};

void Widget::removeChildWidget(Widget *ptr){
    for(auto it = children.begin();it != children.end();it++){
        if(ptr == *it){
            rootView->removeSubView(ptr->rootView.get());
            children.erase(it);
            ptr->notifyObservers(Detach,this);
            ptr->layerTree->notifyObserversOfWidgetDetach();
            break;
        };
    };
};

void Widget::setParentWidget( Widget * widget){
    assert(widget != nullptr && "Cannot set Widget as child of a null Widget");

    if(parent != nullptr){
        parent->removeChildWidget(this);
    }
    parent = widget;
    parent->children.push_back(this);
    setTreeHostRecurse(widget->treeHost);
    parent->rootView->addSubView(rootView.get());
    notifyObservers(Attach,(void *)widget);
};

Widget::~Widget(){
  
};

WidgetObserver::WidgetObserver():hasAssignment(false),widget(nullptr){

};

}
