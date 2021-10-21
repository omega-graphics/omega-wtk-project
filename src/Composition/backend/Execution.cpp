#include "../Compositor.h"
#include "VisualTree.h"
#include <utility>

namespace OmegaWTK::Composition {


void Compositor::executeCurrentCommand(){

    if(currentCommand->type == CompositorCommand::Render) {
        
        auto comm = (CompositionRenderCommand *)currentCommand.get();


        BackendCompRenderTarget *target;
        bool _buildVisualTree = false;

        /// 1. Locate / Create View Render Target for Layer Render

        auto found = renderTargetStore.store.find(comm->renderTarget);
        if (found == renderTargetStore.store.end()) {
             _buildVisualTree = true;
             auto renderTarget = std::dynamic_pointer_cast<ViewRenderTarget>(comm->renderTarget);
             auto visualTree = BackendVisualTree::Create(renderTarget);
             BackendCompRenderTarget compRenderTarget {visualTree};
             renderTargetStore.store.insert(std::make_pair(comm->renderTarget,compRenderTarget));
        } else {
            target = &renderTargetStore.store[comm->renderTarget];
        };

        /// 2. Locate / Create Layer Render Target in Visual Tree.
        BackendRenderTargetContext *targetContext;

        auto layer_found = target->surfaceTargets.find(comm->frame->targetLayer);
        if(layer_found == target->surfaceTargets.end()){
            auto layer = comm->frame->targetLayer;
            auto v = target->visualTree->makeVisual(layer->getLayerRect(),layer->getLayerRect().pos);
            if(layer->isChildLayer()){
                if(!target->visualTree->hasRootVisual()){
                    auto * treeRoot = layer->getParentLimb()->getRootLayer();
                    auto root_v = target->visualTree->makeVisual(treeRoot->getLayerRect(),treeRoot->getLayerRect().pos);
                    target->visualTree->setRootVisual(root_v);
                }

                target->visualTree->addVisual(v);

            }
            else {
                target->visualTree->setRootVisual(v);
            }
            target->surfaceTargets.insert(std::make_pair(layer,&v->renderTarget));
        }
        else {
            targetContext = layer_found->second;
        }


        OmegaCommon::ArrayRef<VisualCommand> commands{comm->frame->currentVisuals};

        /// TODO: Process render commands!

        auto & bkgrd = comm->frame->background;

        targetContext->clear(bkgrd.r,bkgrd.g,bkgrd.b,bkgrd.a);

        for(auto & c : commands){
            targetContext->renderToTarget(c.type,c.params);
        }

        for(auto & effect : comm->frame->currentEffects){
            targetContext->applyEffectToTarget(effect.type,effect.params);
        }

        targetContext->commit();
    }
    else if(currentCommand->type == CompositorCommand::Layer){
        auto params = (CompositorLayerCommand *)currentCommand.get();
        /// Resize Command
        if(params->subtype == CompositorLayerCommand::Resize){
            auto layerRect = params->layer->getLayerRect();
            layerRect.pos.x += (float)params->delta_x;
            layerRect.pos.y += (float)params->delta_y;
            layerRect.w += (float)params->delta_w;;
            layerRect.h += (float)params->delta_h;
            params->layer->resize(layerRect);
        }
        else {

            auto viewRenderTarget = renderTargetStore.store.find(params->parentTarget);
            /// Effect Command
        }
    }

    currentCommand->status.set(CommandStatus::Ok);
};


};