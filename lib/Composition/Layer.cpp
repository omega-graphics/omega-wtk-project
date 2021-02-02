#include "omegaWTK/Composition/Layer.h"
#include <iostream>

namespace OmegaWTK::Composition {
Target::Target(Native::NativeItemPtr _native):native(_native){};
Target::~Target(){};
    
    Layer::Layer(const Core::Rect & rect,Native::NativeItemPtr native_ptr):compTarget(std::make_unique<Target>(native_ptr)),surface_rect(rect){};

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

void Layer::setBackgroundColor(const Color & color){
    background = color;
};

void LayerTree::_recursive_trav(LayerTreeTraversalCallback &callback,Layer *current){
    for(auto & child_layer : current->children){
        if(callback(child_layer)) {
            return;
            break;
        }
        if(!child_layer->children.empty()){
            return _recursive_trav(callback,child_layer);
        };
    };
};

void LayerTree::traverse(LayerTreeTraversalCallback callback){
    if(callback(root))
        return;
    if(!root->children.empty())
        return _recursive_trav(callback,root);
};

bool LayerTree::hasChildLayer(Layer *layer){
    bool returncode = false;
    traverse([&](Layer *current){
        if(current == layer){
            returncode = true;
            return true;
        }
        else
            return false;
    });
    return returncode;
};

LayerTree::LayerTree(Layer *_root):root(_root){};

}
