#include "omegaWTK/UI/View.h"

#include <utility>
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK {


    View::View(const Core::Rect & rect,Composition::LayerTree *layerTree,ViewPtr parent):
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

View::View(const Core::Rect & rect,Native::NativeItemPtr nativeItem,Composition::LayerTree *layerTree,ViewPtr parent):
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
    layerTreeLimb->addLayer(layer);
    return layer;
};

SharedHandle<Composition::Canvas> View::makeCanvas(SharedHandle<Composition::Layer> &targetLayer){
    return std::shared_ptr<Composition::Canvas>(new Composition::Canvas(proxy,*targetLayer));
}

void View::startCompositionSession(){
    proxy.beginRecord();
}

void View::endCompositionSession(){
    proxy.endRecord();
}

void View::enable() {
    renderTarget->getNativePtr()->enable();
}

void View::disable() {
    renderTarget->getNativePtr()->disable();
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

void ViewDelegate::setForwardDelegate(ViewDelegate *delegate){
    forwardDelegate = delegate;
};

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
            if(forwardDelegate != nullptr){
                forwardDelegate->onMouseEnter(event);
            }
            break;
        }
        case NativeEvent::CursorExit : {
            onMouseExit(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onMouseExit(event);
            }
            break;
        }
        case NativeEvent::LMouseDown : {
            onLeftMouseDown(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onLeftMouseDown(event);
            }
            break;
        }
        case NativeEvent::LMouseUp : {
            onLeftMouseUp(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onLeftMouseUp(event);
            }
            break;
        }
        case NativeEvent::RMouseDown : {
            onRightMouseDown(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onRightMouseDown(event);
            }
            break;
        };
        case NativeEvent::RMouseUp : {
            onRightMouseUp(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onRightMouseUp(event);
            }
            break;
        }
        case NativeEvent::KeyDown : {
            onKeyDown(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onKeyDown(event);
            }
            break;
        };
        case NativeEvent::KeyUp : {
            onKeyUp(event);
            if(forwardDelegate != nullptr){
                forwardDelegate->onKeyUp(event);
            }
            break;
        }
        case Native::NativeEvent::ViewResize : {
            auto params = (Native::ViewResize *)event->params;
            view->getRect() = params->rect;
            break;
        }
        
        default:
            break;
    }
};

ScrollView::ScrollView(const Core::Rect & rect, SharedHandle<View> child, bool hasVerticalScrollBar, bool hasHorizontalScrollBar, Composition::LayerTree *layerTree, View *parent):
        View(rect,Native::make_native_item(rect,Native::ScrollItem),layerTree,parent),
        delegate(nullptr),
        hasHorizontalScrollBar(hasHorizontalScrollBar), hasVerticalScrollBar(hasVerticalScrollBar){
    renderTarget->getNativePtr()->event_emitter = this;
    Native::NativeItemPtr ptr = renderTarget->getNativePtr();
    ptr->setClippedView(child->renderTarget->getNativePtr());
    if(hasHorizontalScrollBar)
        ptr->toggleHorizontalScrollBar(hasHorizontalScrollBar);
    if(hasVerticalScrollBar)
        ptr->toggleVerticalScrollBar(hasVerticalScrollBar);
};

bool ScrollView::hasDelegate(){
    return delegate != nullptr;
};

void ScrollView::toggleVerticalScrollBar(){
    renderTarget->getNativePtr()->toggleVerticalScrollBar(hasVerticalScrollBar);
}

void ScrollView::toggleHorizontalScrollBar(){
    renderTarget->getNativePtr()->toggleHorizontalScrollBar(hasHorizontalScrollBar);
}

void ScrollView::setDelegate(ScrollViewDelegate *_delegate){
    delegate = _delegate;
    delegate->scrollView = this;
    setReciever(delegate);
};

void ScrollViewDelegate::onRecieveEvent(Native::NativeEventPtr event){

};

SharedHandle<ClickableViewHandler> ClickableViewHandler::Create() {
    return (SharedHandle<ClickableViewHandler>)new ClickableViewHandler();
}

void ClickableViewHandler::onClick(std::function<void()> handler) {
    click_handler = std::move(handler);
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

SharedHandle<Composition::Brush> cursorBrush = Composition::ColorBrush({Composition::Color::Black8});

TextView::TextView(const Core::Rect &rect,Composition::LayerTree * layerTree,View *parent, bool io) :
View(rect,layerTree,parent),
textRect(
        Composition::TextRect::Create(
        rect,
        Composition::TextLayoutDescriptor {
            Composition::TextLayoutDescriptor::LeftUpper,
            Composition::TextLayoutDescriptor::WrapByWord
        })),
        rootLayerCanvas(makeCanvas(getLayerTreeLimb()->getRootLayer())),
        str(),
        editMode(io),cursorLayer(makeLayer({{0,0},100,100})),
        cursorCanvas(makeCanvas(cursorLayer)){

        
}

void TextView::moveTextCursorToMousePoint(Core::Position & pos){
    
}

void TextView::enableCursor(){
    if(font){
        cursorLayer->setEnabled(true);
        auto &r = cursorLayer->getLayerRect();
        cursorCanvas->drawRect(r,cursorBrush);
        cursorCanvas->sendFrame();
    }
}

void TextView::disableCursor(){
    if(font){
        cursorLayer->setEnabled(false);
    }
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
    rootLayerCanvas->drawGETexture(img.s,getRect(),img.textureFence);
    rootLayerCanvas->sendFrame();
}

void TextView::updateFont(SharedHandle<Composition::Font> &font) {
    this->font = font;
}

void TextView::setContent(const UChar *str){
    this->str.setTo(str,u_strlen(str));
    commitChanges();
}

TextViewDelegate::TextViewDelegate(TextView *view) : ViewDelegate(){
    view->setDelegate(this);
    clickHandler = std::make_unique<ClickableViewHandler>();
    setForwardDelegate(clickHandler.get());
     clickHandler->onClick([&](){
         auto _v = (TextView *)this->view;
        Core::Position pos {};
        _v->startCompositionSession();
        _v->moveTextCursorToMousePoint(pos);
        _v->enableCursor();
        _v->endCompositionSession();
    });
}


void TextViewDelegate::onKeyDown(Native::NativeEventPtr event){
    auto _v = (TextView *)this->view;
    UChar32 ch;
    _v->startCompositionSession();
    _v->pushChar(ch);
    _v->commitChanges();
    _v->endCompositionSession();
}

void TextViewDelegate::onKeyUp(Native::NativeEventPtr event){
    
}





};
