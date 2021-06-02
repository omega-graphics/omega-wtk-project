#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Composition/ViewRenderTarget.h"
#include "omegaWTK/UI/VideoView.h"
#include "omegaWTK/UI/WidgetTreeHost.h"

namespace OmegaWTK {


Widget::Widget(const Core::Rect & rect,SharedHandle<Widget> parent,WidgetTreeHost *parentHost,Composition::Compositor *compositor):parent(parent),treeHost(parentHost),compositor(compositor){
    layerTree = std::make_shared<Composition::LayerTree>(compositor);
    rootView = std::make_shared<CanvasView>(rect,layerTree.get(),nullptr);
    // std::cout << "Constructing View for Widget" << std::endl;
    if(parent)
        parent->rootView->addSubView(this->rootView.get());
//    std::cout << "RenderTargetPtr:" << rootView->renderTarget.get() << std::endl;
};

SharedHandle<Composition::Layer> Widget::makeLayer(Core::Rect rect){
    return std::make_shared<Composition::Layer>(rect,compositor);
};

//Widget::Widget(Widget & widget):parent(std::move(widget.parent)),compositor(std::move(widget.compositor)),rootView(std::move(widget.rootView)){
//    
//};

SharedHandle<View> Widget::makeCanvasView(const Core::Rect & rect,View *parent){
    return std::make_shared<CanvasView>(rect,layerTree.get(),parent);
};

//SharedHandle<VideoView> Widget::makeVideoView(const Core::Rect & rect,View * parent){
//    return std::make_shared<VideoView>(rect,compositor,parent);
//};

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

void Widget::refresh(){
    compositor->updateLayerTree(layerTree.get());
};

void Widget::addObserver(WidgetObserver * observer){
    if(!observer->hasAssignment) {
        observers.push_back(observer);
        observer->hasAssignment = true;
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

void Widget::notifyObservers(Widget::WidgetEventType event_ty,Core::Rect * rect){
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
                observer->onWidgetChangeSize(*rect,rootView->rect);
                break;
            }
        }
    };
};

Widget::~Widget(){
  
};

WidgetObserver::WidgetObserver():hasAssignment(false),widget(nullptr){

};

}
