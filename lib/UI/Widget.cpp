#include "omegaWTK/UI/Widget.h"
#include "omegaWTK/Composition/ViewRenderTarget.h"
#include "omegaWTK/UI/VideoView.h"

namespace OmegaWTK {

Widget::Widget(const Core::Rect & rect,SharedHandle<Widget> parent):parent(parent),compositor(new Composition::Compositor()){
    layerTree = std::make_shared<Composition::LayerTree>(compositor);
    rootView = std::make_shared<CanvasView>(rect,layerTree.get(),nullptr);
    if(parent)
        parent->rootView->addSubView(this->rootView.get());
//    std::cout << "RenderTargetPtr:" << rootView->renderTarget.get() << std::endl;
};

SharedHandle<Composition::Layer> Widget::makeLayer(const Core::Rect & rect){
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

void Widget::show(){
    rootView->renderTarget->getNativePtr()->enable();
};
void Widget::hide(){
    rootView->renderTarget->getNativePtr()->disable();
};

Widget::~Widget(){
  
};

}
