#include "omegaWTK/UI/View.h"
#include "omegaWTK/Composition/CompositorClient.h"

namespace OmegaWTK {


    View::View(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent):
        renderTarget(std::make_shared<Composition::ViewRenderTarget>(Native::make_native_item(rect))),
        proxy(std::static_pointer_cast<Composition::CompositionRenderTarget>(renderTarget)),
        widgetLayerTree(layerTree),
        parent_ptr(parent),
        rect(rect){

        layerTreeLimb = widgetLayerTree->createLimb(rect);
        renderTarget->getNativePtr()->setLayerTreeLimb(layerTreeLimb.get());
        renderTarget->getNativePtr()->event_emitter = this;
        
        if(parent_ptr) {
            parent->addSubView(this);
            layerTree->addChildLimb(layerTreeLimb,parent->layerTreeLimb.get());
        }
        else
            layerTree->setRootLimb(layerTreeLimb);
    };
//    View::View(const Core::Rect & rect,View *parent):
//        renderTarget(std::make_shared<Composition::ViewRenderTarget>(Native::make_native_item(rect))),
//        proxy(std::static_pointer_cast<Composition::CompositionRenderTarget>(renderTarget)),
//        widgetLayerTree(nullptr),
//        parent_ptr(parent),
//        rect(rect){
//
//        renderTarget->getNativePtr()->event_emitter = this;
//        if(parent_ptr) {
//            parent->addSubView(this);
//        };
//    };
    bool View::hasDelegate(){
        return delegate != nullptr;
    };
    void View::setDelegate(ViewDelegate *_delegate){
        delegate = _delegate;
        delegate->view = this;
        setReciever(delegate);
    };
    void View::addSubView(View * view){
        subviews.emplace_back(view);
        renderTarget->getNativePtr()->addChildNativeItem(view->renderTarget->getNativePtr());
    };
    void View::removeSubView(View *view){
        auto it  = subviews.begin();
        while(it != subviews.end()){
            auto v = *it;
            if(v.get() == view){
                subviews.erase(it);
                renderTarget->getNativePtr()->removeChildNativeItem(view->renderTarget->getNativePtr());
                view->parent_ptr = nullptr;
                return;
                break;
            };
        };
    };

    void View::resize(Core::Rect newRect){
        renderTarget->getNativePtr()->resize(newRect);
    };

View::View(const Core::Rect & rect,Native::NativeItemPtr nativeItem,Composition::LayerTree *layerTree,View *parent):
rect(rect),
widgetLayerTree(layerTree),
renderTarget(std::make_shared<Composition::ViewRenderTarget>(nativeItem)),
proxy(std::static_pointer_cast<Composition::CompositionRenderTarget>(renderTarget)),
parent_ptr(parent){
    if(parent_ptr) {
        parent->addSubView(this);
    };
};

SharedHandle<Composition::Layer> View::makeLayer(Core::Rect rect){
    auto layer = std::make_shared<Composition::Layer>(rect);
    layer->parentLimb = layerTreeLimb.get();
    return layer;
};

SharedHandle<Composition::Canvas> View::makeCanvas(SharedHandle<Composition::Layer> &targetLayer){
    return std::make_shared<Composition::Canvas>(proxy,*targetLayer);
}

void View::startCompositionSession(){
    proxy.beginRecord();
}

void View::endCompositionSession(){
    proxy.endRecord();
}

View::~View(){
    std::cout << "View will destruct" << std::endl;
};

void View::setFrontendRecurse(Composition::Compositor *frontend){
    proxy.setFrontendPtr(frontend);
    for(auto & subView : subviews){
        subView->setFrontendRecurse(frontend);
    };
};
    
// Composition::Compositor * View::getWidgetCompositor(){
//     return widgetLayerTree->widgetCompositor;
// };

ViewDelegate::ViewDelegate(){};

ViewDelegate::~ViewDelegate(){};

void ViewDelegate::onRecieveEvent(Native::NativeEventPtr event){
    using Native::NativeEvent;
    switch (event->type) {
//        case NativeEvent::HasLoaded : {
//            viewHasLoaded(event);
//            break;
//        }
        case NativeEvent::CursorEnter : {
            onMouseEnter(event);
            break;
        }
        case NativeEvent::CursorExit : {
            onMouseExit(event);
            break;
        }
        case NativeEvent::LMouseDown : {
            onLeftMouseDown(event);
            break;
        }
        case NativeEvent::LMouseUp : {
            onLeftMouseUp(event);
            break;
        }
        case NativeEvent::RMouseDown : {
            onRightMouseDown(event);
            break;
        };
        case NativeEvent::RMouseUp : {
            onRightMouseUp(event);
            break;
        }
        case NativeEvent::KeyDown : {
            onKeyDown(event);
            break;
        };
        case NativeEvent::KeyUp : {
            onKeyUp(event);
            break;
        }
        
        default:
            break;
    }
};

ScrollView::ScrollView(const Core::Rect & rect,SharedHandle<View> child,bool hasVericalScrollBar,bool hasHorizontalScrollBar,Composition::LayerTree *layerTree,View *parent):
View(rect,Native::make_native_item(rect,Native::ScrollItem),layerTree,parent),
delegate(nullptr),
hasHorizontalScrollBar(hasHorizontalScrollBar),hasVericalScrollBar(hasVericalScrollBar){
    renderTarget->getNativePtr()->event_emitter = this;
    Native::NativeItemPtr ptr = renderTarget->getNativePtr();
    ptr->setClippedView(child->renderTarget->getNativePtr());
    if(hasHorizontalScrollBar)
        ptr->toggleHorizontalScrollBar(hasHorizontalScrollBar);
    if(hasVericalScrollBar)
        ptr->toggleVerticalScrollBar(hasVericalScrollBar);
};

bool ScrollView::hasDelegate(){
    return delegate != nullptr;
};

void ScrollView::setDelegate(ViewDelegate *_delegate){
    ScrollViewDelegate *d = (ScrollViewDelegate *)_delegate;

    delegate = d;
    delegate->scrollView = this;
    setReciever(delegate);
};

void ScrollViewDelegate::onRecieveEvent(Native::NativeEventPtr event){

};

SharedHandle<ClickableViewHandler> ClickableViewHandler::Create() {
    return (SharedHandle<ClickableViewHandler>)new ClickableViewHandler();
}

void ClickableViewHandler::onClick(std::function<void()> handler) {
    click_handler = handler;
}

void ClickableViewHandler::onHoverBegin(std::function<void()> handler) {
    hover_begin_handler = handler;
}

void ClickableViewHandler::onHoverEnd(std::function<void()> handler) {
    hover_end_handler = handler;
}

void ClickableViewHandler::onPress(std::function<void()> handler) {
    press_handler = handler;
}

void ClickableViewHandler::onRelease(std::function<void()> handler) {
    release_handler = handler;
}

void ClickableViewHandler::onMouseEnter(Native::NativeEventPtr event) {
    hover_begin_handler();
}

void ClickableViewHandler::onMouseExit(Native::NativeEventPtr event) {
    hover_end_handler();
}

void ClickableViewHandler::onLeftMouseDown(Native::NativeEventPtr event) {
    press_handler();
    click_handler();
}

void ClickableViewHandler::onLeftMouseUp(Native::NativeEventPtr event) {
    release_handler();
}

TextView::TextView(const Core::Rect &rect,Composition::LayerTree * layerTree,View *parent, bool io) :
View(rect,layerTree,parent),
editMode(io),str(),
textRect(
        Composition::TextRect::Create(
        rect,
        Composition::TextLayoutDescriptor {
            Composition::TextLayoutDescriptor::LeftUpper,
            Composition::TextLayoutDescriptor::WrapByWord
        })),
        rootLayerCanvas(makeCanvas(getLayerTreeLimb()->getRootLayer())){

}

void TextView::pushChar(Unicode32Char &ch) {
    str.append(ch);
}

void TextView::popChar() {
    str.truncate(str.length() - 1);
}

void TextView::commitChanges() {
    auto run = Composition::GlyphRun::fromUStringAndFont(str,font);
    textRect->drawRun(run,Composition::Color::create8Bit(Composition::Color::Eight::Black8));
    auto img = textRect->toBitmap();
    rootLayerCanvas->drawGETexture(img,getRect());
    rootLayerCanvas->sendFrame();
}

void TextView::updateFont(SharedHandle<Composition::Font> &font) {
    this->font = font;
}





};
