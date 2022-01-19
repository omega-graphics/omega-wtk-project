#include "../Compositor.h"
#include "VisualTree.h"
#include <utility>

namespace OmegaWTK::Composition {


void Compositor::executeCurrentCommand(){

    if(currentCommand->type == CompositorCommand::Render) {

        OMEGAWTK_DEBUG("Rendering Frame!!")
        
        auto comm = (CompositionRenderCommand *)currentCommand.get();


        BackendCompRenderTarget *target = nullptr;
        bool _buildVisualTree = false;

        /// 1. Locate / Create View Render Target for Layer Render

        auto found = renderTargetStore.store.find(comm->renderTarget);
        if (found == renderTargetStore.store.end()) {
             _buildVisualTree = true;
             auto renderTarget = std::dynamic_pointer_cast<ViewRenderTarget>(comm->renderTarget);
             auto visualTree = BackendVisualTree::Create(renderTarget);
             BackendCompRenderTarget compRenderTarget {visualTree};
             target = &renderTargetStore.store.insert(std::make_pair(comm->renderTarget,compRenderTarget)).first->second;
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
                    auto treeRoot = layer->getParentLimb()->getRootLayer();
                    auto root_v = target->visualTree->makeVisual(treeRoot->getLayerRect(),treeRoot->getLayerRect().pos);
                    target->visualTree->setRootVisual(root_v);
                }

                target->visualTree->addVisual(v);

            }
            else {
                target->visualTree->setRootVisual(v);
            }
            targetContext = target->surfaceTargets.insert(std::make_pair(layer,&v->renderTarget)).first->second;
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
        OMEGAWTK_DEBUG("Committed Data!")
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

            auto s = viewRenderTarget->second.surfaceTargets[params->layer];
            if(s == &(viewRenderTarget->second.visualTree->root->renderTarget)){
                if(params->effect->type == LayerEffect::DropShadow){
                    viewRenderTarget->second.visualTree->root->updateShadowEffect(*(LayerEffect::DropShadowParams *)params->effect->params);
                }
                else {
                    viewRenderTarget->second.visualTree->root->updateTransformEffect(*(LayerEffect::TransformationParams *)params->effect->params);
                }

            }
            else {
                for(auto & v : viewRenderTarget->second.visualTree->body){
                    if(s == &(v->renderTarget)){
                        if(params->effect->type == LayerEffect::DropShadow){
                            v->updateShadowEffect(*(LayerEffect::DropShadowParams *)params->effect->params);
                        }
                        else {
                            v->updateTransformEffect(*(LayerEffect::TransformationParams *)params->effect->params);
                        }
                        break;
                    }
                }
            }
        }
    }
    else if(currentCommand->type == CompositorCommand::View){
        auto params = (CompositorViewCommand *)currentCommand.get();
        if(params->subType == CompositorViewCommand::Resize){
            auto rect = params->viewPtr->getRect();
            params->viewPtr->resize(Core::Rect {Core::Position {rect.pos.x + params->delta_x,rect.pos.y + params->delta_y},rect.w + params->delta_w,rect.h + params->delta_h});
        }
    }
    else if(currentCommand->type == CompositorCommand::Cancel){
        auto params = (CompositorCancelCommand *)currentCommand.get();
        /// Filter commands that have the id in order to cancel execution of them.
        commandQueue.filter([&](SharedHandle<CompositorCommand> & command){
            return command->id >= params->startID && command->id <= params->endID;
        });
    }

    currentCommand->status.set(CommandStatus::Ok);
};


};