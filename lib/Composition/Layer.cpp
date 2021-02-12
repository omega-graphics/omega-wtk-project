#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Compositor.h"
#include <iostream>

namespace OmegaWTK::Composition {


Target::Target(Native::NativeItemPtr _native) : native(_native){};
Target::~Target(){};

void Layer::drawRect(const Core::Rect &rect, const Color &color,Core::Optional<Border> border) {
  compTarget->visuals.push_back(new Visual(
      {compTarget->id_gen,Visual::Rect, (void *)new Visual::RectParams({rect, color,border})}));
  ++compTarget->id_gen;
};

void Layer::drawRoundedRect(const Core::RoundedRect &rect, const Color &color,Core::Optional<Border> border){
  compTarget->visuals.push_back(new Visual(
    {compTarget->id_gen,Visual::RoundedRect,(void *) new Visual::RoundedRectParams({{rect.pos,rect.dimen},rect.radius_x,rect.radius_y,color,border})}));
    ++compTarget->id_gen;
};

void Layer::drawText(const Core::String & str,unsigned size,const Color & color,const Core::Rect & rect,const Text::Font & font){
    compTarget->visuals.push_back(new Visual(
      {compTarget->id_gen,Visual::Text,(void *) new Visual::TextParams({Text({str,size,font}),color,rect})}));
    ++compTarget->id_gen;
};

void Layer::drawEllipse(const Core::Ellipse &ellipse,const Color &color,Core::Optional<Border> border){
  compTarget->visuals.push_back(new Visual(
    {compTarget->id_gen,Visual::Ellipse,(void *) new Visual::EllipseParams({ellipse,color,border})}));
  ++compTarget->id_gen;
};

void Layer::drawBitmap(BitmapImage image,const Core::Rect & rect){
  compTarget->visuals.push_back(new Visual(
    {compTarget->id_gen,Visual::Bitmap,(void *)new Visual::BitmapParams({image,rect})
  }));
  ++compTarget->id_gen;
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
