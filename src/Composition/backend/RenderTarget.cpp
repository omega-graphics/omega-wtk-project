#include "RenderTarget.h"

namespace OmegaWTK::Composition {

    OmegaGTE::SharedHandle<OmegaGTE::GEFunctionLibrary> stdlibrary;
    OmegaGTE::SharedHandle<OmegaGTE::GERenderPipelineState> pipelineState;
    
    GERenderTargetContext::GERenderTargetContext(OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> & renderTarget):
    renderTarget(renderTarget),__hasQueuedRenderJobs(false),tessContext(gte.tessalationEngine->createTEContextFromNativeRenderTarget(renderTarget)){
        
    };
    
    bool GERenderTargetContext::hasQueuedRenderJobs(){
        return __hasQueuedRenderJobs;
    };

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
        }
    };

    OmegaGTE::GERenderTarget *GERenderTargetContext::getRenderTarget(){
        return renderTarget.get();
    };

    void GERenderTargetContext::commit(){
        renderTarget->commitAndPresent();
    };

    void BackendCompRenderTarget::renderVisualTree(){

    };

}