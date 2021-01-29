#include "omegaWTK/Composition/Layer.h"
#include <iostream>

namespace OmegaWTK::Composition {
    Layer::Layer(const Core::Rect & rect,Native::NativeItemPtr native_ptr):native_binding(native_ptr),surface_rect(rect){};

    void Layer::addSubLayer(Layer *layer){
        layer->parent_ptr = this;
        children.push_back(layer);
    };

    void Layer::removeSubLayer(Layer *layer){
        auto it = children.begin();
        while(it != children.end()){
            auto l = *it;
            if(l == layer){
                children.erase(it);
                layer->parent_ptr = nullptr;
                return;
                break;
            };
            ++it;
        };
        std::cout << "Error! Could not Remove Sublayer!" << std::endl;
        
    };

}