#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Compositor.h"
#include <iostream>

namespace OmegaWTK::Composition {

#define VISUAL_SET_PARAMS(arg)                                                 \
  switch (type) {                                                              \
  case Rect: {                                                                 \
    RectParams *params = (RectParams *)this->params;                           \
                                                                               \
    arg break;                                                                 \
  };                                                                           \
  case Text: {                                                                 \
    TextParams *params = (TextParams *)this->params;                           \
    arg break;                                                                 \
  }                                                                            \
  case RoundedRect: {                                                          \
    RoundedRectParams *params = (RoundedRectParams *)this->params;             \
    arg break;                                                                 \
  }                                                                            \
  case Ellipse: {                                                              \
    EllipseParams *params = (EllipseParams *)this->params;                     \
    arg break;                                                                 \
  }                                                                            \
  };

void Visual::setColor(const Color &new_color) {
  VISUAL_SET_PARAMS(params->color = std::move(new_color);)
};
void Visual::setFont(const Text::Font &new_font){
    if(type == Text){
        TextParams *params = (TextParams *)this->params;
        params->text.setFont(std::move(new_font));
    };
};
void Visual::setRect(const Core::Rect &new_rect){
    
};

VPVR Visual::getRect(){

};

VPVR Visual::getColor(){

};

VPVR Visual::getFont(){

};

Target::Target(Native::NativeItemPtr _native) : native(_native){};
Target::~Target(){};

void Layer::drawRect(const Core::Rect &rect, const Color &color) {
  compTarget->visuals.push_back(new Visual(
      {Visual::Rect, (void *)new Visual::RectParams({rect, color})}));
};

Layer::Layer(const Core::Rect &rect, Native::NativeItemPtr native_ptr,
             Compositor *compPtr)
    : surface_rect(rect), compTarget(new Target(native_ptr)),
      ownerCompositor(compPtr) {
  native_ptr->setParentLayer(this);
};

void Layer::addSubLayer(Layer *layer) {
  layer->parent_ptr = this;
  children.push_back(layer);
};

void Layer::removeSubLayer(Layer *layer) {
  auto it = children.begin();
  while (it != children.end()) {
    auto l = *it;
    if (l == layer) {
      children.erase(it);
      layer->parent_ptr = nullptr;
      return;
      break;
    };
    ++it;
  };
  std::cout << "Error! Could not Remove Sublayer!" << std::endl;
};

void Layer::setBackgroundColor(const Color &color) { background = color; };

void Layer::redraw() { ownerCompositor->updateRequestLayer(this); };

Layer::~Layer() { delete compTarget; };

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
