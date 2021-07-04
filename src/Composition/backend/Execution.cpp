#include "../Compositor.h"

namespace OmegaWTK::Composition {

OmegaGTE::NativeRenderTargetDescriptor * makeDescForViewRenderTarget(ViewRenderTarget *renderTarget);


void Compositor::layerHasResized(Layer *layer){

};

void Compositor::layerHasDisabled(Layer *layer){

};

void Compositor::layerHasEnabled(Layer *layer){

};

std::future<RenderCommandStatus> Compositor::executeCurrentRenderCommand(){

    BackendCompRenderTarget *target;

    auto found = renderTargetStore.store.find(currentCommand->renderTarget);
    if(found == renderTargetStore.store.end()){

        auto renderTarget = (ViewRenderTarget *)currentCommand->renderTarget;
        auto desc = makeDescForViewRenderTarget(renderTarget);
        auto geTarget = gte.graphicsEngine->makeNativeRenderTarget(*desc);
        BackendCompRenderTarget compTarget {{geTarget}};
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
            OmegaGTE::TextureRenderTargetDescriptor desc;
            desc.rect.pos.x = desc.rect.pos.y = 0;
            desc.rect.w = surface->getParentLayer()->getLayerRect().w;
            desc.rect.h = surface->getParentLayer()->getLayerRect().h;

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

            auto textureTarget = gte.graphicsEngine->makeTextureRenderTarget(desc);
            GERenderTargetContext ctxt {textureTarget};
            target->surfaceTargets.insert(std::make_pair(surface,ctxt));
            surfaceRenderTargetCtxt = &target->surfaceTargets[surface];
        }
        else {
            surfaceRenderTargetCtxt = &target->surfaceTargets[surface];
        };
    };

    auto commandStatus = status_promise.get_future();

    return commandStatus;
};


};