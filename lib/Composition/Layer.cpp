#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Compositor.h"
#include <iostream>

namespace OmegaWTK::Composition {


Target::Target(Native::NativeItemPtr _native) : native(_native),style(nullptr){};
Target::~Target(){};

Layer::Layer(const Core::Rect &rect, Native::NativeItemPtr native_ptr,
             Compositor *compPtr)
    : surface_rect(rect), compTarget(std::make_unique<Target>(native_ptr)),
      ownerCompositor(compPtr) {
  native_ptr->setParentLayer(this);
};

void Layer::addSubLayer(Layer *layer) {
  layer->parent_ptr = this;
  children.push_back(layer);
  compTarget->native->addChildNativeItem(layer->getTargetNativePtr());
};

void Layer::removeSubLayer(Layer *layer) {
  auto it = children.begin();
  while (it != children.end()) {
    auto l = *it;
    if (l == layer) {
      children.erase(it);
      layer->parent_ptr = nullptr;
      compTarget->native->removeChildNativeItem(layer->getTargetNativePtr());
      return;
      break;
    };
    ++it;
  };
  std::cout << "Error! Could not Remove Sublayer!" << std::endl;
};

void Layer::setBackgroundColor(const Color &color) { background = color; };

void Layer::redraw() { ownerCompositor->updateRequestLayer(this); };

Layer::~Layer() { };

void LayerTree::_recursive_trav(LayerTreeTraversalCallback &callback,
                                Layer *current) {
  for (auto &child_layer : current->children) {
    if (callback(child_layer)) {
      return;
      break;
    }
    if (!child_layer->children.empty()) {
      return _recursive_trav(callback, child_layer);
    };
  };
};

void LayerTree::traverse(LayerTreeTraversalCallback callback) {
  if (callback(root))
    return;
  if (!root->children.empty())
    return _recursive_trav(callback, root);
};

bool LayerTree::hasChildLayer(Layer *layer) {
  bool returncode = false;
  traverse([&](Layer *current) {
    if (current == layer) {
      returncode = true;
      return true;
    } else
      return false;
  });
  return returncode;
};

LayerTree::LayerTree(Layer *_root) : root(_root){};

} // namespace OmegaWTK::Composition
