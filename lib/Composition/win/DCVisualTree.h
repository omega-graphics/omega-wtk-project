#include "omegaWTK/Core/Core.h"
#include <memory>
#include <functional>

#include <wrl.h>
#include <dcomp.h>

#ifndef OMEGAWTK_COMPOSITION_WIN_DCVISUALTREE_H
#define OMEGAWTK_COMPOSITION_WIN_DCVISUALTREE_H

namespace OmegaWTK::Composition {

using Microsoft::WRL::ComPtr;

class DCVisual {
    ComPtr<IDCompositionVisual> native;
    ComPtr<IDCompositionSurface> content;
    Core::Vector<DCVisual *> children;
    bool layerChildrenAbove;
    friend class DCVisualTree;
    public:
    void addChild(DCVisual *ptr);
    void removeChild(DCVisual *ptr);
    // Success: Returns pointer to `DCVisual` 
    // Fails: Returns `nullptr`
    DCVisual * getChildAtIdx(unsigned idx);
    void setContent(IDCompositionSurface *surface_ptr);
    DCVisual() = delete;
    DCVisual(DCVisual &&) = delete;
    DCVisual(IDCompositionVisual *ptr,bool _layerChildrenAbove);
    ~DCVisual();
};

typedef std::function<void(DCVisual *)> DCVisualTreeTraversalCallback;

class DCVisualTree {
    std::shared_ptr<DCVisual> root;
    void _recurse_trav(DCVisualTreeTraversalCallback &callback,DCVisual *parent);
    public:
    void traverse(DCVisualTreeTraversalCallback callback);
    DCVisualTree() = delete;
    DCVisualTree(const DCVisualTree &&) = delete;
    DCVisualTree(DCVisual *ptr);
};

};

#endif