#include "omegaWTK/UI/View.h"
#include "omegaWTK/Composition/ViewRenderTarget.h"

namespace OmegaWTK {


    View::View(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent):renderTarget(std::make_shared<Composition::ViewRenderTarget>(Native::make_native_item(rect))),widgetLayerTree(layerTree),parent_ptr(parent),rect(rect){
        layerTreeLimb = widgetLayerTree->createLimb(rect,renderTarget.get());
        Native::set_native_item_event_emitter(renderTarget->getNativePtr(),this);
        
        if(parent_ptr) {
            parent->addSubView(this);
            layerTree->addChildLimb(layerTreeLimb,parent->layerTreeLimb.get());
        }
        else
            layerTree->setRootLimb(layerTreeLimb);
    };
    View::View(const Core::Rect & rect,View *parent):renderTarget(std::make_unique<Composition::ViewRenderTarget>(Native::make_native_item(rect))),widgetLayerTree(nullptr),parent_ptr(parent),rect(rect){
        Native::set_native_item_event_emitter(renderTarget->getNativePtr(),this);
        if(parent_ptr) {
            parent->addSubView(this);
        };
    };
    bool View::hasDelegate(){
        return delegate != nullptr;
    };
    void View::setDelegate(ViewDelegate *_delegate){
        delegate = _delegate;
        delegate->view = this;
        setReciever(delegate);
    };
    void View::addSubView(View * view){
        subviews.push_back(SharedHandle<View>(view));
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

View::View(const Core::Rect & rect,Native::NativeItemPtr nativeItem,View *parent):rect(rect),widgetLayerTree(nullptr),renderTarget(std::make_unique<Composition::ViewRenderTarget>(nativeItem)),parent_ptr(parent){
    if(parent_ptr) {
        parent->addSubView(this);
    };
};

View::~View(){
    std::cout << "View will destruct" << std::endl;
};
    
Composition::Compositor * View::getWidgetCompositor(){
    return widgetLayerTree->widgetCompositor;
};

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
    /// Garbage Collect!
    delete event;
};

ScrollView::ScrollView(const Core::Rect & rect,SharedHandle<View> child,bool hasVericalScrollBar,bool hasHorizontalScrollBar,View *parent):View(rect,Native::make_native_item(rect,Native::ScrollItem),parent),delegate(nullptr),hasHorizontalScrollBar(hasHorizontalScrollBar),hasVericalScrollBar(hasVericalScrollBar){
    Native::set_native_item_event_emitter(renderTarget->getNativePtr(),this);
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


};
