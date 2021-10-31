#ifndef RUNTIME_SHADER_COMP_SUPPORT
#define RUNTIME_SHADER_COMP_SUPPORT 1
#endif

#include "RenderTarget.h"
#include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK::Composition {

    static OmegaGTE::SharedHandle<OmegaGTE::GTEShaderLibrary> shaderLibrary;
    static OmegaGTE::SharedHandle<OmegaGTE::GEBufferWriter> bufferWriter;
    static OmegaGTE::SharedHandle<OmegaGTE::GERenderPipelineState> renderPipelineState;
    static OmegaGTE::SharedHandle<OmegaGTE::GERenderPipelineState> textureRenderPipelineState;

    static OmegaGTE::SharedHandle<OmegaGTE::GEComputePipelineState> linearGradientPipelineState;

    OmegaCommon::String librarySource = R"(

struct GradientTextureConstParams {
    float arg;
};

struct LinearGradientStop {
    float pos;
    float4 color;
};

buffer<GradientTextureConstParams> input : 5;

buffer<LinearGradientStop> stops : 3;
texture2d outputTex : 4;

[in input,in stops,out outputTex]
compute(x=1,y=1,z=1)
void linearGradient(uint3 thread_id : GlobalThreadID){

}

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

        renderPipelineDescriptor.vertexFunc = shaderLibrary->shaders["textureVertex"];
        renderPipelineDescriptor.fragmentFunc = shaderLibrary->shaders["textureFragment"];
        textureRenderPipelineState = gte.graphicsEngine->makeRenderPipelineState(renderPipelineDescriptor);

        OmegaGTE::ComputePipelineDescriptor linearGradientPipelineDesc {};
        linearGradientPipelineDesc.computeFunc = shaderLibrary->shaders["linearGradient"];
        linearGradientPipelineState = gte.graphicsEngine->makeComputePipelineState(linearGradientPipelineDesc);
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

    OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc {};
    renderPassDesc.colorAttachment = new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment(
            OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::ClearColor(r,g,b,a),
            OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::Clear);
    renderPassDesc.depthStencilAttachment.disabled = true;
    cb->startRenderPass(renderPassDesc);
    cb->endRenderPass();
}

void BackendRenderTargetContext::applyEffectToTarget(CanvasEffect::Type type, void *params) {
    effectQueue.push_back(std::make_pair(type,params));
}

    struct FinalTextureDrawVertex {
        OmegaGTE::FVec<3> pos;
        OmegaGTE::FVec<2> coord;
    };

    const FinalTextureDrawVertex vertices[] = {
            {
                OmegaGTE::FVec<3>::Create(),
                OmegaGTE::FVec<2>::Create()
            }
    };

    static OmegaGTE::SharedHandle<OmegaGTE::GEBuffer> finalTextureDrawBuffer;

    void BackendRenderTargetContext::commit(){
        OmegaGTE::SharedHandle<OmegaGTE::GETexture> dest;
        imageProcessor->applyEffects(dest,preEffectTarget,effectQueue);
        effectQueue.clear();
        auto cb = renderTarget->commandBuffer();
        renderTarget->notifyCommandBuffer(cb,imageProcessor->fence);
        OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc {};
        cb->startRenderPass(renderPassDesc);
        cb->setRenderPipelineState(textureRenderPipelineState);
        cb->bindResourceAtVertexShader(finalTextureDrawBuffer,1);
        cb->bindResourceAtFragmentShader(dest,2);
        cb->drawPolygons(OmegaGTE::GERenderTarget::CommandBuffer::Triangle,6,0);
        cb->endRenderPass();
        renderTarget->submitCommandBuffer(cb);
        renderTarget->commitAndPresent();
    }

    void
    BackendRenderTargetContext::createGradientTexture(bool linearOrRadial, Gradient &gradient, OmegaGTE::GRect &rect,
                                                      OmegaGTE::SharedHandle<OmegaGTE::GETexture> &dest) {
        auto cb = renderTarget->commandBuffer();

        size_t structSize = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT});

        OmegaGTE::BufferDescriptor bufferDescriptor {OmegaGTE::BufferDescriptor::Upload,structSize,structSize,OmegaGTE::Shared};

        auto constBuffer = gte.graphicsEngine->makeBuffer(bufferDescriptor);


        bufferWriter->setOutputBuffer(constBuffer);
        bufferWriter->structBegin();
        bufferWriter->writeFloat(gradient.angle);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();
        bufferWriter->flush();

        structSize =  OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT,OMEGASL_FLOAT4});

        bufferDescriptor.len = structSize * gradient.stops.size();
        bufferDescriptor.objectStride = structSize;

        auto stopsBuffer = gte.graphicsEngine->makeBuffer(bufferDescriptor);

        bufferWriter->setOutputBuffer(stopsBuffer);

        for(auto & stop : gradient.stops){
            bufferWriter->structBegin();
            bufferWriter->writeFloat(stop.pos);
            auto vec_color = OmegaGTE::makeColor(stop.color.r,stop.color.g,stop.color.b,stop.color.a);
            bufferWriter->writeFloat4(vec_color);
            bufferWriter->structEnd();
            bufferWriter->sendToBuffer();
        }

        bufferWriter->flush();

        cb->startComputePass(linearGradientPipelineState);
        cb->bindResourceAtComputeShader(dest,4);
        cb->bindResourceAtComputeShader(constBuffer,5);
        cb->bindResourceAtComputeShader(stopsBuffer,3);
        cb->dispatchThreads((unsigned)rect.w,(unsigned)rect.h,1);
        cb->endComputePass();
        renderTarget->submitCommandBuffer(cb);
    }

    void BackendRenderTargetContext::renderToTarget(VisualCommand::Type type, void *params) {
        OmegaGTE::TETessellationResult result;

        size_t struct_size;
        bool useTextureRenderPipeline = false;

        switch (type) {
            case VisualCommand::Rect : {
                auto _params = (VisualCommand::RectParams *)params;
                auto te_params = OmegaGTE::TETessellationParams::Rect(_params->rect);
                result = tessellationEngineContext->tessalateSync(te_params);

                useTextureRenderPipeline = _params->brush->isColor;

                break;
            }
            case VisualCommand::RoundedRect : {
                auto _params = (VisualCommand::RoundedRectParams *)params;
                auto te_params = OmegaGTE::TETessellationParams::RoundedRect(_params->rect);
                result = tessellationEngineContext->tessalateSync(te_params);

                useTextureRenderPipeline = _params->brush->isColor;

                break;
            }
        }

        if(useTextureRenderPipeline){
            struct_size = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT3,OMEGASL_FLOAT2});
        }
        else {
            struct_size = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT3,OMEGASL_FLOAT4});
        }

        OmegaGTE::BufferDescriptor bufferDesc {OmegaGTE::BufferDescriptor::Upload,result.totalVertexCount() *struct_size,struct_size};
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
        unsigned startVertexIndex = 0;
        for(auto & m : result.meshes) {
            OmegaGTE::GERenderTarget::CommandBuffer::PolygonType topology;
            if(m.topology == OmegaGTE::TETessellationResult::TEMesh::TopologyTriangleStrip){
                topology = OmegaGTE::GERenderTarget::CommandBuffer::TriangleStrip;
            }
            else {
                topology = OmegaGTE::GERenderTarget::CommandBuffer::Triangle;
            }
            cb->drawPolygons(topology, m.vertexCount(), startVertexIndex);
            startVertexIndex += m.vertexCount();
        }
        cb->endRenderPass();
        renderTarget->submitCommandBuffer(cb);
    }



}