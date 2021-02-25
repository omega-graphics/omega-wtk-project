#include "DCVisualTree.h"
#include <iostream>

namespace OmegaWTK::Composition {
    
    DCVisual::DCVisual(IDCompositionVisual *ptr,bool _layerChildrenAbove):native(ptr),layerChildrenAbove(_layerChildrenAbove){

    };
    
    void DCVisual::setContent(IDCompositionSurface *surface_ptr){
        native->SetContent(surface_ptr);
        content = surface_ptr;
    };

    void DCVisual::addChild(DCVisual *ptr){
        BOOL placeAbove = layerChildrenAbove;
        HRESULT hr = native->AddVisual(ptr->native.Get(),placeAbove,native.Get());
        if(!SUCCEEDED(hr))
            std::cout << "ERROR:" << GetLastError() << std::endl;
        children.push_back(ptr);
    };

    void DCVisual::removeChild(DCVisual *ptr){
        HRESULT hr = native->RemoveVisual(ptr->native.Get());
        if(!SUCCEEDED(hr))
            std::cout << "ERROR:" << GetLastError() << std::endl;
        auto it = children.begin();
        while(it != children.end()){
            auto test = *it;
            if(test == ptr){
                children.erase(it);
                break;
            };
        };

    };

    DCVisual * DCVisual::getChildAtIdx(unsigned idx){
        auto it = children.begin();
        while(it != children.end()){
            if(idx == 0){
                return *it;
                break;
            };
            ++it;
            --idx;
        };
        return nullptr;
    };

    DCVisual::~DCVisual(){
        native->RemoveAllVisuals();
        auto n_ptr =native.Detach();
        auto c_ptr = content.Detach();
        Core::SafeRelease(&n_ptr);
        Core::SafeRelease(&c_ptr);
    };

    DCVisualTree::DCVisualTree(DCVisual *ptr):root(ptr){

    };

    void DCVisualTree::_recurse_trav(DCVisualTreeTraversalCallback &callback,DCVisual *parent){
        for(auto & child : parent->children){
            callback(child);
            if(!child->children.empty())
                _recurse_trav(callback,child);
        };
    };

    void DCVisualTree::traverse(DCVisualTreeTraversalCallback callback){
        _recurse_trav(callback,root.get());
    };

};