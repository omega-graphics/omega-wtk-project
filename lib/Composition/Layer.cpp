#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/Compositor.h"
#include <iostream>

namespace OmegaWTK::Composition {


ViewRenderTarget::ViewRenderTarget(Native::NativeItemPtr _native) : native(_native){};
Native::NativeItemPtr ViewRenderTarget::getNativePtr(){ return native;};
ViewRenderTarget::~ViewRenderTarget(){};

Layer::Layer(const Core::Rect &rect,
             Compositor *compPtr)
    : surface_rect(rect),
      ownerCompositor(compPtr),parent_ptr(nullptr) {
  
};

void Layer::addSubLayer(SharedHandle<Layer> &layer) {
  layer->parent_ptr = (this);
  children.push_back(layer);
//  compTarget->native->addChildNativeItem(layer->getTargetNativePtr());
};

void Layer::removeSubLayer(SharedHandle<Layer> &layer) {
  auto it = children.begin();
  while (it != children.end()) {
    auto l = *it;
    if (l == layer) {
      children.erase(it);
      layer->parent_ptr = nullptr;
//      compTarget->native->removeChildNativeItem(layer->getTargetNativePtr());
      return;
      break;
    };
    ++it;
  };
  std::cout << "Error! Could not Remove Sublayer!" << std::endl;
};

void Layer::setStyle(SharedHandle<LayerStyle> & style){
    this->style = style;
};

//void Layer::redraw() { ownerCompositor->updateRequestedLayer(this); };

Layer::~Layer() { };


LayerTree::LayerTree(Compositor *compPtr):widgetCompositor(compPtr),rootLimb(nullptr){};

void LayerTree::addChildLimb(SharedHandle<LayerTree::Limb> & limb,Limb *parent){
    if(!parent)
        parent = rootLimb.get();
    
    auto it = body.find(parent);
    if(it == body.end()){
        /// If Limb has never been a parent.
        body.insert(std::make_pair(parent,Core::Vector<SharedHandle<Limb>>({limb})));
    }
    else {
        /// If the limb is currently a parent of other limbs.
        it->second.push_back(limb);
    };
};

void LayerTree::setRootLimb(SharedHandle<Limb> & limb){
    rootLimb = limb;
};

LayerTree::Limb::Limb(const Core::Rect &rect,Compositor *compPtr,ViewRenderTarget *renderTarget):limbRoot(new Layer(rect,compPtr)),enabled(true),renderTarget(renderTarget){
    renderTarget->getNativePtr()->setLayerTreeLimb(this);
};

void LayerTree::Limb::addLayer(SharedHandle<Layer> layer){
    limbRoot->addSubLayer(layer);
};

void LayerTree::Limb::redraw(){
    limbRoot->ownerCompositor->updateRequestedLayerTreeLimb(this);
};

LayerTree::Limb::iterator LayerTree::Limb::begin(){
    return limbRoot->children.begin();
};

LayerTree::Limb::iterator LayerTree::Limb::end(){
    return limbRoot->children.end();
};

void LayerTree::Limb::enable(){
    if(!enabled)
        limbRoot->setEnabled(true);
};

void LayerTree::Limb::disable(){
    if(enabled)
        limbRoot->setEnabled(false);
};

Layer * LayerTree::Limb::getRootLayer(){
    return limbRoot;
};

LayerTree::Limb * LayerTree::getTreeRoot(){
    return rootLimb.get();
};

unsigned LayerTree::getParentLimbChildCount(LayerTree::Limb * parent){
    return body[parent].size();
};

LayerTree::Limb * LayerTree::getLimbAtIndexFromParent(unsigned idx,Limb *parent){
    return body[parent][idx].get();
};

LayerTree::~LayerTree(){
    
};

SharedHandle<LayerTree::Limb> LayerTree::createLimb(const Core::Rect & rect,ViewRenderTarget *renderTarget){
    return std::make_shared<LayerTree::Limb>(rect,widgetCompositor,renderTarget);
};


WindowLayer::WindowLayer(Core::Rect & rect,Native::NWH native_window_ptr):native_window_ptr(native_window_ptr),rect(rect){
    // MessageBoxA(HWND_DESKTOP,"Creating Window Layer!","NOTE",MB_OK);
//    native_window_ptr->setNativeLayer(this);
};

void WindowLayer::setWindowStyle(SharedHandle<WindowStyle> & style){
    this->style = style;
};

void WindowLayer::redraw(){
    
};

} // namespace OmegaWTK::Composition
