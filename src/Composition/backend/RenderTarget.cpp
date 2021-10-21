#ifndef RUNTIME_SHADER_COMP_SUPPORT
#define RUNTIME_SHADER_COMP_SUPPORT 1
#endif

#include "RenderTarget.h"
#include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK::Composition {

    static OmegaGTE::SharedHandle<OmegaGTE::GTEShaderLibrary> shaderLibrary;
    static OmegaGTE::SharedHandle<OmegaGTE::GEBufferWriter> bufferWriter;
    static OmegaGTE::SharedHandle<OmegaGTE::GERenderPipelineState> renderPipelineState;

    OmegaCommon::String librarySource = R"(

 struct OmegaWTKColoredVertex {
   float3 pos;
   float4 color;
 };

 struct OmegaWTKColoredRasterData internal {
   float4 pos : Position;
   float4 color : Color;
 };

buffer<OmegaWTKColoredVertex> v_buffer : 0;

[in v_buffer]
vertex OmegaWTKColoredRasterData mainVertex(uint v_id : VertexID){
    OmegaWTKColoredVertex v = v_buffer[v_id];
    OmegaWTKColoredRasterData rasterData;
    rasterData.pos = float4(v.pos,1.f);
    rasterData.color = v.color;
    return rasterData;
}

fragment float4 mainFragment(OmegaWTKColoredRasterData raster){
    return raster.color;
}

struct OmegaWTKTexturedVertex {
    float3 pos;
    float2 texCoord;
};

struct OmegaWTKTexturedRasterData internal {
    float4 pos : Position;
    float2 texCoord : TexCoord;
};

buffer<OmegaWTKTexturedVertex> v_buffer_1 : 1;

[in v_buffer_1]
vertex OmegaWTKTexturedRasterData vertexTexture(uint v_id : VertexID){
    OmegaWTKTexturedVertex v = v_buffer_1[v_id];
    OmegaWTKTexturedRasterData rasterData;
    rasterData.pos = float4(v.pos);
    rasterData.texCoord = v.texCoord;
    return rasterData;
}

texture2d tex : 2;
static sampler2d mainSampler(filer=linear);

[in tex,in mainSampler]
fragment float4 fragmentTexture(OmegaGTETexturedRasterData raster){
    return sample(mainSampler,tex,raster.texCoord);
}

)";

    void loadGlobalRenderAssets(){
        auto compiler = OmegaSLCompiler::Create();
        auto library = compiler->compile({OmegaSLCompiler::Source::fromString(librarySource)});
        shaderLibrary = gte.graphicsEngine->loadShaderLibraryRuntime(library);
        OmegaGTE::RenderPipelineDescriptor renderPipelineDescriptor {};
        renderPipelineDescriptor.cullMode = OmegaGTE::RasterCullMode::None;
        renderPipelineDescriptor.depthAndStencilDesc = {false,false};
        renderPipelineDescriptor.triangleFillMode = OmegaGTE::TriangleFillMode::Solid;
        renderPipelineDescriptor.rasterSampleCount = 1;
        renderPipelineDescriptor.vertexFunc = shaderLibrary->shaders["mainVertex"];
        renderPipelineDescriptor.fragmentFunc = shaderLibrary->shaders["mainFragment"];
        renderPipelineState = gte.graphicsEngine->makeRenderPipelineState(renderPipelineDescriptor);
    }

BackendRenderTargetContext::BackendRenderTargetContext(Core::Rect & rect,
        OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> &renderTarget):
        renderTarget(renderTarget),
        tessellationEngineContext(gte.tessalationEngine->createTEContextFromNativeRenderTarget(renderTarget)),
        renderTargetSize(rect)
        {
    if(!bufferWriter){
        bufferWriter = OmegaGTE::GEBufferWriter::Create();
    }
}

    void BackendRenderTargetContext::setRenderTargetSize(Core::Rect &rect) {
        renderTargetSize = rect;
    }

void BackendRenderTargetContext::clear(float r, float g, float b, float a) {
    auto cb = renderTarget->commandBuffer();

    OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc {new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment(
            OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::ClearColor(r,g,b,a),
            OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::Clear)};
    cb->startRenderPass(renderPassDesc);
    cb->endRenderPass();
}

void BackendRenderTargetContext::applyEffectToTarget(CanvasEffect::Type type, void *params) {

}

void BackendRenderTargetContext::commit(){
        renderTarget->commitAndPresent();
    }

void BackendRenderTargetContext::renderToTarget(VisualCommand::Type type, void *params) {
    switch (type) {
        case VisualCommand::Rect : {
            auto _params = (VisualCommand::RectParams *)params;
            auto te_params = OmegaGTE::TETessellationParams::Rect(_params->rect);
            auto result = tessellationEngineContext->tessalateSync(te_params,nullptr);

            size_t struct_size = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT3,OMEGASL_FLOAT4});

            OmegaGTE::BufferDescriptor bufferDesc {OmegaGTE::BufferDescriptor::Upload,6 *struct_size,struct_size};
            auto buffer = gte.graphicsEngine->makeBuffer(bufferDesc);

            auto cb = renderTarget->commandBuffer();
            OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc {};

            OmegaGTE::GEViewport viewport {};
            viewport.x = 0;
            viewport.y = 0;
            viewport.farDepth = 1.f;
            viewport.nearDepth = 0.f;
            viewport.width = renderTargetSize.w;
            viewport.height = renderTargetSize.h;
            OmegaGTE::GEScissorRect scissorRect {0,0,renderTargetSize.w,renderTargetSize.h};

            renderPassDesc.colorAttachment = new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment(OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::ClearColor(1.f,1.f,1.f,1.f),OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::Load);
            cb->startRenderPass(renderPassDesc);
            cb->setRenderPipelineState(renderPipelineState);
            cb->bindResourceAtVertexShader(buffer,0);
            cb->setViewports({viewport});
            cb->setScissorRects({scissorRect});
            cb->drawPolygons(OmegaGTE::GERenderTarget::CommandBuffer::Triangle,6,0);
            cb->endRenderPass();

            break;
        }
    }
}



}