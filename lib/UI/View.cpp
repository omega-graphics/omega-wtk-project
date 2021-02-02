#include "omegaWTK/UI/View.h"

namespace OmegaWTK {
    LayerOwner::LayerOwner(const Core::Rect & rect,Composition::Layer * layer):layer(layer){};

    View::View(const Core::Rect & rect,Composition::Layer *layer_to_use,Native::NativeItemPtr item):LayerOwner(rect,layer_to_use),rect(rect),native(item){
        Native::set_native_item_event_emitter(native,this);
    };
    bool View::hasDelegate(){
        return delegate != nullptr;
    };
    void View::setDelegate(ViewDelegate *_delegate){
        delegate = _delegate;
        delegate->view = this;
    };
    void View::addSubView(View *view){
        subviews.push_back(view);
        view->parent_ptr = this;
        this->layer->addSubLayer(view->layer);
    };
    void View::removeSubView(View *view){
        auto it  = subviews.begin();
        while(it != subviews.end()){
            auto v = *it;
            if(v == view){
                subviews.erase(it);
                view->parent_ptr = nullptr;
                this->layer->removeSubLayer(view->layer);
                return;
                break;
            };
        };
    };

    View *make_view(const Core::Rect & rect){
        Native::NativeItemPtr native = Native::make_native_item(rect);
        return new View(rect,new Composition::Layer(rect,native),native);
    };
};
