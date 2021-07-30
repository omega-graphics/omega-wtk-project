#include "omegaWTK/Composition/Layer.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include <iostream>

namespace OmegaWTK::Composition {

LayerTreeObserver::~LayerTreeObserver(){
    
};

Layer::Layer(const Core::Rect &rect)
    :
      surface(std::make_shared<CanvasSurface>(surface_rect)),surface_rect(rect),needsNativeResize(false){
          surface->parentLayer = this;
};

void Layer::setCompClientRecurse(Composition::CompositorClient *compClient){
    surface->client = compClient;
    for(auto & c : children){
        c->setCompClientRecurse(compClient);
    };
}

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

// void Layer::setStyle(SharedHandle<LayerStyle> & style){
//     this->style = style;
// };

SharedHandle<CanvasSurface> & Layer::getSurface(){
    return surface;
};

void Layer::resize(Core::Rect &newRect){
    surface_rect = newRect;
    parentLimb->getParentTree()->notifyObserversOfResize(this);
};

LayerTree::Limb * Layer::getParentLimb(){
    return parentLimb;
};

Layer::~Layer() { };


LayerTree::LayerTree():rootLimb(nullptr){};

void LayerTree::addChildLimb(SharedHandle<LayerTree::Limb> & limb,Limb *parent){
    if(!parent)
        parent = rootLimb.get();
    
    auto it = body.find(parent);
    if(it == body.end()){
        /// If Limb has never been a parent.
        body.insert(std::make_pair(parent,OmegaCommon::Vector<SharedHandle<Limb>>({limb})));
    }
    else {
        /// If the limb is currently a parent of other limbs.
        it->second.push_back(limb);
    };
};

void LayerTree::setRootLimb(SharedHandle<Limb> & limb){
    rootLimb = limb;
};

void LayerTree::notifyObserversOfResize(Layer *layer){
    for(auto & observer : observers){
        observer->layerHasResized(layer);
    };
};

void LayerTree::notifyObserversOfDisable(Layer *layer){
    for(auto & observer : observers){
        observer->layerHasDisabled(layer);
    };
};

void LayerTree::notifyObserversOfEnable(Layer *layer){
    for(auto & observer : observers){
        observer->layerHasEnabled(layer);
    };
};

void LayerTree::notifyObserversOfWidgetDetach(){
    for(auto & observer : observers){
        observer->hasDetached(this);
    };
};

void LayerTree::setCompClientRecurse(CompositorClient *compClient){
    rootLimb->getRootLayer()->setCompClientRecurse(compClient);
    for(auto & b : body){

    };
};

LayerTree::Limb::Limb(const Core::Rect &rect,ViewRenderTarget *renderTarget):limbRoot(new Layer(rect)),enabled(true),renderTarget(renderTarget){
    renderTarget->getNativePtr()->setLayerTreeLimb(this);
};

void LayerTree::Limb::addLayer(SharedHandle<Layer> layer){
    limbRoot->addSubLayer(layer);
};

// void LayerTree::Limb::commit(){
    
// };

// void LayerTree::Limb::redraw(){
//     limbRoot->ownerCompositor->updateRequestedLayerTreeLimb(this);
// };

// void LayerTree::Limb::layout(){
//     limbRoot->ownerCompositor->layoutLayerTreeLimb(this);
// };

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

LayerTree *LayerTree::Limb::getParentTree(){
    return parentTree;
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

void LayerTree::addObserver(LayerTreeObserver * observer){
    observers.push_back(observer);
};

void LayerTree::removeObserver(LayerTreeObserver * observer){
    for(auto it = observers.begin();it != observers.end();it++){
        if(observer == *it){
            observers.erase(it);
            break;
        }
    };
};

LayerTree::~LayerTree(){
    
};

SharedHandle<LayerTree::Limb> LayerTree::createLimb(const Core::Rect & rect,ViewRenderTarget *renderTarget){
    auto limb = std::make_shared<LayerTree::Limb>(rect,renderTarget);
    limb->parentTree = this;
    return limb;
};


WindowLayer::WindowLayer(Core::Rect & rect,Native::NWH native_window_ptr):native_window_ptr(native_window_ptr),rect(rect){
    // MessageBoxA(HWND_DESKTOP,"Creating Window Layer!","NOTE",MB_OK);
//    native_window_ptr->setNativeLayer(this);
};

// void WindowLayer::setWindowStyle(SharedHandle<WindowStyle> & style){
//     this->style = style;
// };

void WindowLayer::redraw(){
    
};

} // namespace OmegaWTK::Composition
