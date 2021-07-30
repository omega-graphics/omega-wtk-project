#include "RenderTarget.h"

namespace OmegaWTK::Composition {

    void RenderTargetStore::cleanTargets(LayerTree *tree,LayerTree::Limb *limb){
        auto found = store.find(limb->renderTarget);
        if(found != store.end()){
            store.erase(found);
        }
        auto c = tree->getParentLimbChildCount(limb);
        for(unsigned i = 0;i < c;i++){
            auto child = tree->getLimbAtIndexFromParent(i,limb);
            cleanTargets(tree,child);
        }
    }

    void RenderTargetStore::cleanTreeTargets(LayerTree *tree){
        auto root = tree->getTreeRoot();
        cleanTargets(tree,root);
    }

    OmegaGTE::SharedHandle<OmegaGTE::GEFunctionLibrary> stdlibrary;
    OmegaGTE::SharedHandle<OmegaGTE::GERenderPipelineState> pipelineState;
    
    GERenderTargetContext::GERenderTargetContext(OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> & renderTarget):
    renderTarget(renderTarget),tessContext(gte.tessalationEngine->createTEContextFromNativeRenderTarget(renderTarget)),__hasQueuedRenderJobs(false){
        
    }
    
    bool GERenderTargetContext::hasQueuedRenderJobs(){
        return __hasQueuedRenderJobs;
    }

    void GERenderTargetContext::renderToTarget(VisualCommand::Type type, void *params){
        if(!__hasQueuedRenderJobs)
            __hasQueuedRenderJobs = true;
        
        switch (type) {
            case VisualCommand::Rect : {
                auto _params = (VisualCommand::RectParams *)params;
                auto tessParams = OmegaGTE::TETessalationParams::Rect(_params->rect);
                
                auto tessRes = tessContext->tessalateSync(tessParams);
                auto v_buffer = tessContext->convertToVertexBuffer(gte.graphicsEngine,tessRes);

                auto commandBuffer = renderTarget->commandBuffer();
                OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc;
                renderPassDesc.colorAttachment = 
                new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment(
                    OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::ClearColor {0.f,0.f,0.f,0.f},
                    OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::Load);
                commandBuffer->startRenderPass(renderPassDesc);
                commandBuffer->setRenderPipelineState(pipelineState);
                commandBuffer->setResourceConstAtVertexFunc(v_buffer,0);
                commandBuffer->drawPolygons(OmegaGTE::GERenderTarget::CommandBuffer::Triangle,v_buffer->size(),0);
                commandBuffer->endRenderPass();

                renderTarget->submitCommandBuffer(commandBuffer);
                break;
            }
            case VisualCommand::RoundedRect : {
                 auto _params = (VisualCommand::RoundedRectParams *)params;
                 OmegaGTE::GRoundedRect rrect;
                 rrect.pos = _params->rect.pos;
                 rrect.w = _params->rect.w;
                 rrect.h = _params->rect.h;
                 rrect.rad_x = _params->rad_x;
                 rrect.rad_y = _params->rad_y;

                auto tessParams = OmegaGTE::TETessalationParams::RoundedRect(rrect);
                
                auto tessRes = tessContext->tessalateSync(tessParams);
                auto v_buffer = tessContext->convertToVertexBuffer(gte.graphicsEngine,tessRes);

                auto commandBuffer = renderTarget->commandBuffer();
                OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc;
                renderPassDesc.colorAttachment = 
                new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment(
                    OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::ClearColor {0.f,0.f,0.f,0.f},
                    OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::Load);
                commandBuffer->startRenderPass(renderPassDesc);
                commandBuffer->setRenderPipelineState(pipelineState);
                commandBuffer->setResourceConstAtVertexFunc(v_buffer,0);
                commandBuffer->drawPolygons(OmegaGTE::GERenderTarget::CommandBuffer::Triangle,v_buffer->size(),0);
                commandBuffer->endRenderPass();

                renderTarget->submitCommandBuffer(commandBuffer);
                break;
            }
        }
    }

    GERenderTargetContext::~GERenderTargetContext(){

    } 

    OmegaGTE::GERenderTarget *GERenderTargetContext::getRenderTarget(){
        return renderTarget.get();
    }

    void GERenderTargetContext::commit(){
        renderTarget->commitAndPresent();
    }

}