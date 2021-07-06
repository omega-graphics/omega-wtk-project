#include "../Compositor.h"
#include "VisualTree.h"
#include <utility>

namespace OmegaWTK::Composition {

OmegaGTE::NativeRenderTargetDescriptor * makeDescForViewRenderTarget(ViewRenderTarget *renderTarget);
OmegaGTE::NativeRenderTargetDescriptor * makeDescForCanvasSurface(CanvasSurface *layer);

void Compositor::layerHasResized(Layer *layer){

};

void Compositor::layerHasDisabled(Layer *layer){

};

void Compositor::layerHasEnabled(Layer *layer){

};

std::future<RenderCommandStatus> Compositor::executeCurrentRenderCommand(){

    BackendCompRenderTarget *target;
    bool __buildVisualTree = false;

    auto found = renderTargetStore.store.find(currentCommand->renderTarget);
    if(found == renderTargetStore.store.end()){
        __buildVisualTree = true;
        auto renderTarget = (ViewRenderTarget *)currentCommand->renderTarget;
        auto desc = makeDescForViewRenderTarget(renderTarget);
        auto geTarget = gte.graphicsEngine->makeNativeRenderTarget(*desc);
        BackendCompRenderTarget compTarget {new GERenderTargetContext{geTarget}};
        compTarget.visualTree = CreateVisualTree();
        renderTargetStore.store.insert(std::make_pair(renderTarget,compTarget));
        target = &renderTargetStore.store[renderTarget];
    }
    else {
        target = &renderTargetStore.store[currentCommand->renderTarget];
    };

    OmegaCommon::ArrayRef<VisualCommand> commands {currentCommand->_visuals,currentCommand->_visuals + currentCommand->visual_count};
    
    std::promise<RenderCommandStatus> status_promise;
    
    for(auto & com : commands){
        GERenderTargetContext *surfaceRenderTargetCtxt;
        auto surface = com.targetSurface;
        auto s_found = target->surfaceTargets.find(surface);
        if(s_found == target->surfaceTargets.end()){
            OmegaGTE::NativeRenderTargetDescriptor *desc = makeDescForCanvasSurface(surface);
            

            auto targetTree = surface->getParentLayer()->getParentLimb()->getParentTree();

            auto tree_it = targetLayerTrees.begin();
            bool found_tree = false;
            while(tree_it != targetLayerTrees.end()){
                if(*tree_it == targetTree) {
                    found_tree = true;
                    break;
                }
                ++tree_it;
            };
            if(!found_tree) {
                targetTree->addObserver(this);
                targetLayerTrees.push_back(targetTree);
            }

            auto _target = gte.graphicsEngine->makeNativeRenderTarget(*desc);
            auto ctxt = new GERenderTargetContext{_target};
            auto visual = target->visualTree->makeVisual(*ctxt,*desc,surface->getParentLayer()->getLayerRect().pos);
            if(surface->getParentLayer()->isChildLayer())
                target->visualTree->addVisual(visual);
            else 
                target->visualTree->setRootVisual(visual);
            target->surfaceTargets.insert(std::make_pair(surface,ctxt));
            surfaceRenderTargetCtxt = target->surfaceTargets[surface];
        }
        else {
            surfaceRenderTargetCtxt = target->surfaceTargets[surface];
        };
        surfaceRenderTargetCtxt->renderToTarget(com.type,com.params);
    };

    for(auto & s_target_pair : target->surfaceTargets){
        auto s_target = s_target_pair.second;
        if(s_target->hasQueuedRenderJobs())
            s_target->commit();
    };

    if(__buildVisualTree)
        target->renderVisualTree();


    auto commandStatus = status_promise.get_future();

    return commandStatus;
};


};