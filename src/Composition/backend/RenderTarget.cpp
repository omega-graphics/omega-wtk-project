

#include "RenderTarget.h"
#include "omegaWTK/Composition/Canvas.h"

#include "omegaWTK/Media/ImgCodec.h"

namespace OmegaWTK::Composition {
    #ifdef TARGET_MACOS
    void stopMTLCapture();
    #endif

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

// [in input,in stops,out outputTex]
// compute(x=1,y=1,z=1)
// void linearGradient(uint3 thread_id : GlobalThreadID){
//
// }

 struct OmegaWTKColoredVertex {
   float4 pos;
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
    rasterData.pos = v.pos;
    rasterData.color = v.color;
    return rasterData;
}

fragment float4 mainFragment(OmegaWTKColoredRasterData raster){
    return raster.color;
}

struct OmegaWTKTexturedVertex {
    float4 pos;
    float2 texCoord;
};

struct OmegaWTKTexturedRasterData internal {
    float4 pos : Position;
    float2 texCoord : TexCoord;
};

buffer<OmegaWTKTexturedVertex> v_buffer_1 : 1;

[in v_buffer_1]
vertex OmegaWTKTexturedRasterData textureVertex(uint v_id : VertexID){
    OmegaWTKTexturedVertex v = v_buffer_1[v_id];
    OmegaWTKTexturedRasterData rasterData;
    rasterData.pos = v.pos;
    rasterData.texCoord = v.texCoord;
    return rasterData;
}

texture2d tex : 2;
static sampler2d mainSampler(filter=linear);

[in tex,in mainSampler]
fragment float4 textureFragment(OmegaWTKTexturedRasterData raster){
    return sample(mainSampler,tex,raster.texCoord);
}

)";

    static OmegaGTE::SharedHandle<OmegaGTE::GEBuffer> finalTextureDrawBuffer;

    void loadGlobalRenderAssets(){
        bufferWriter = OmegaGTE::GEBufferWriter::Create();
        auto & compiler = gte.omegaSlCompiler;
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

//        OmegaGTE::ComputePipelineDescriptor linearGradientPipelineDesc {};
//        linearGradientPipelineDesc.computeFunc = shaderLibrary->shaders["linearGradient"];
//        linearGradientPipelineState = gte.graphicsEngine->makeComputePipelineState(linearGradientPipelineDesc);
        auto struct_size = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT4,OMEGASL_FLOAT2});

        auto pos = OmegaGTE::FVec<4>::Create();
        auto texCoord = OmegaGTE::FVec<2>::Create();
        pos[0][0] = -1.f;
        pos[1][0] = 1.f;
        pos[2][0] = 0.f;
        pos[3][0] = 1.f;

        texCoord[0][0] = 0.f;
        texCoord[1][0] = 0.f;

        finalTextureDrawBuffer = gte.graphicsEngine->makeBuffer({OmegaGTE::BufferDescriptor::Upload,struct_size * 6,struct_size});
        bufferWriter->setOutputBuffer(finalTextureDrawBuffer);
        /// Triangle 1
        bufferWriter->structBegin();
        bufferWriter->writeFloat4(pos);
        bufferWriter->writeFloat2(texCoord);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();

        texCoord[1][0] = 1.f;
        pos[1][0] = -1.f;

        bufferWriter->structBegin();
        bufferWriter->writeFloat4(pos);
        bufferWriter->writeFloat2(texCoord);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();

        texCoord[0][0] = 1.f;
        pos[0][0] = 1.f;

        bufferWriter->structBegin();
        bufferWriter->writeFloat4(pos);
        bufferWriter->writeFloat2(texCoord);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();


        /// Triangle 2

        texCoord[0][0] = texCoord[1][0] = 0.f;
        pos[1][0] = 1.f;
        pos[0][0] = -1.f;

        bufferWriter->structBegin();
        bufferWriter->writeFloat4(pos);
        bufferWriter->writeFloat2(texCoord);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();

        texCoord[0][0] = 1.f;
        pos[0][0] = 1.f;

        bufferWriter->structBegin();
        bufferWriter->writeFloat4(pos);
        bufferWriter->writeFloat2(texCoord);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();

        texCoord[1][0] = 1.f;
        pos[1][0] = -1.f;

        bufferWriter->structBegin();
        bufferWriter->writeFloat4(pos);
        bufferWriter->writeFloat2(texCoord);
        bufferWriter->structEnd();
        bufferWriter->sendToBuffer();

        bufferWriter->flush();
    }

    void destroyGlobalRenderAssets(){
        shaderLibrary.reset();
        renderPipelineState.reset();
        textureRenderPipelineState.reset();
        linearGradientPipelineState.reset();
        bufferWriter.reset();
        finalTextureDrawBuffer.reset();
    }

    void InitializeEngine(){
        loadGlobalRenderAssets();
    }

    void CleanupEngine(){
        destroyGlobalRenderAssets();
    }

BackendRenderTargetContext::BackendRenderTargetContext(Core::Rect & rect,
        OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> &renderTarget):
        fence(gte.graphicsEngine->makeFence()),
        renderTarget(renderTarget),
        renderTargetSize(rect)
        {
    // if(!bufferWriter){
    //     bufferWriter = OmegaGTE::GEBufferWriter::Create();
    // }
    OmegaGTE::TextureDescriptor textureDescriptor {};
    textureDescriptor.usage = OmegaGTE::GETexture::RenderTarget;
    textureDescriptor.storage_opts = OmegaGTE::Shared;
    textureDescriptor.width = (unsigned)renderTargetSize.w;
    textureDescriptor.height = (unsigned)renderTargetSize.h;
    textureDescriptor.type = OmegaGTE::GETexture::Texture2D;
    textureDescriptor.pixelFormat = OmegaGTE::TexturePixelFormat::RGBA8Unorm;

    targetTexture = gte.graphicsEngine->makeTexture(textureDescriptor);

    preEffectTarget = gte.graphicsEngine->makeTextureRenderTarget({true,targetTexture});

    tessellationEngineContext = gte.tessalationEngine->createTEContextFromTextureRenderTarget(preEffectTarget);

    imageProcessor = BackendCanvasEffectProcessor::Create(fence);
}

    void BackendRenderTargetContext::setRenderTargetSize(Core::Rect &rect) {
        renderTargetSize = rect;
    }

void BackendRenderTargetContext::clear(float r, float g, float b, float a) {
    auto cb = preEffectTarget->commandBuffer();

    OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc {};
    renderPassDesc.colorAttachment = new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment(
            OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::ClearColor(r,g,b,a),
            OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::Clear);
    renderPassDesc.depthStencilAttachment.disabled = true;
    cb->startRenderPass(renderPassDesc);
    cb->endRenderPass();
    preEffectTarget->submitCommandBuffer(cb);
}

void BackendRenderTargetContext::applyEffectToTarget(CanvasEffect::Type type, void *params) {
    effectQueue.push_back(std::make_pair(type,params));
}


    void BackendRenderTargetContext::commit(){
        auto _l_cb = preEffectTarget->commandBuffer();
        preEffectTarget->submitCommandBuffer(_l_cb,fence);
        preEffectTarget->commit();

        // @brief FIX Metal Render Target First then Try This Block
        auto cb = renderTarget->commandBuffer();
        // OmegaGTE::SharedHandle<OmegaGTE::GETexture> & dest = targetTexture;

        // if(!effectQueue.empty()) {
        //     imageProcessor->applyEffects(dest, preEffectTarget, effectQueue);
        //     effectQueue.clear();
        // }

        renderTarget->notifyCommandBuffer(cb, fence);
        OmegaGTE::GERenderTarget::RenderPassDesc renderPassDesc {};
        renderPassDesc.depthStencilAttachment.disabled = true;

        OmegaGTE::GEViewport viewport {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.farDepth = 1.f;
        viewport.nearDepth = 0.f;
        viewport.width = renderTargetSize.w;
        viewport.height = renderTargetSize.h;
        OmegaGTE::GEScissorRect scissorRect {0,0,renderTargetSize.w,renderTargetSize.h};

        renderPassDesc.colorAttachment = new OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment{{0.f,0.f,0.f,0.f},OmegaGTE::GERenderTarget::RenderPassDesc::ColorAttachment::LoadAction::LoadPreserve};
        cb->startRenderPass(renderPassDesc);
        cb->setViewports({viewport});
        cb->setScissorRects({scissorRect});
        cb->setRenderPipelineState(textureRenderPipelineState);
        cb->bindResourceAtVertexShader(finalTextureDrawBuffer,1);
        auto t = preEffectTarget->underlyingTexture();
        cb->bindResourceAtFragmentShader(t,2);
        cb->drawPolygons(OmegaGTE::GERenderTarget::CommandBuffer::Triangle,6,0);
        cb->endRenderPass();
        renderTarget->submitCommandBuffer(cb);
        renderTarget->commitAndPresent();

        #ifdef TARGET_MACOS

        stopMTLCapture();
        
        #endif
    }

    void
    BackendRenderTargetContext::createGradientTexture(bool linearOrRadial, Gradient &gradient, OmegaGTE::GRect &rect,
                                                      OmegaGTE::SharedHandle<OmegaGTE::GETexture> &dest) {
        auto cb = renderTarget->commandBuffer();

        size_t structSize = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT});

        OmegaGTE::BufferDescriptor bufferDescriptor {OmegaGTE::BufferDescriptor::Upload,structSize,structSize,OmegaGTE::Shared};

        auto constBuffer = gte.graphicsEngine->makeBuffer(bufferDescriptor);


//        bufferWriter->setOutputBuffer(constBuffer);
//        bufferWriter->structBegin();
//        bufferWriter->writeFloat(gradient);
//        bufferWriter->structEnd();
//        bufferWriter->sendToBuffer();
//        bufferWriter->flush();

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

        OmegaGTE::GEViewport viewPort {};
        viewPort.x = viewPort.y = viewPort.nearDepth = 0.f;
        viewPort.farDepth = 1.f;
        viewPort.width = renderTargetSize.w;
        viewPort.height = renderTargetSize.h;

        std::cout << "W:" << renderTargetSize.w << " H:" << renderTargetSize.h << std::endl;

        size_t struct_size;
        bool useTextureRenderPipeline = false;

        OmegaGTE::SharedHandle<OmegaGTE::GETexture> texturePaint;

        OmegaGTE::SharedHandle<OmegaGTE::GEFence> textureFence;

        switch (type) {
            case VisualCommand::Rect : {
                auto _params = (VisualCommand::RectParams *)params;
                OmegaGTE::GRect r{OmegaGTE::GPoint2D {0,0},_params->rect.w,_params->rect.h};
                auto te_params = OmegaGTE::TETessellationParams::Rect(r);

                useTextureRenderPipeline = !_params->brush->isColor;

                if(!useTextureRenderPipeline){
                    auto color = OmegaGTE::makeColor(_params->brush->color.r,
                                                     _params->brush->color.g,
                                                     _params->brush->color.b,
                                                     _params->brush->color.a);
                    te_params.addAttachment(OmegaGTE::TETessellationParams::Attachment::makeColor(color));
                }

                result = tessellationEngineContext->tessalateSync(te_params,OmegaGTE::GTEPolygonFrontFaceRotation::Clockwise,&viewPort);
                result.translate(-((viewPort.width/2) - _params->rect.pos.x),
                                 -((viewPort.height/2) - _params->rect.pos.y),
                                 0,
                                 viewPort);

                break;
            }
            case VisualCommand::Bitmap : {
                auto _params = (VisualCommand::BitmapParams *)params;
                OmegaGTE::GRect r{OmegaGTE::GPoint2D {0,0},_params->rect.w,_params->rect.h};
                auto te_params = OmegaGTE::TETessellationParams::Rect(r);

                useTextureRenderPipeline = true;
                if(_params->texture){
                    texturePaint = _params->texture;
                    textureFence = _params->textureFence;
                }
                else {
                    OmegaGTE::TextureDescriptor texDesc {OmegaGTE::GETexture::Texture2D};
                    texDesc.usage = OmegaGTE::GETexture::ToGPU;
                    texDesc.width = _params->img->header.width;
                    texDesc.height = _params->img->header.height;
                    std::cout << "TEX W:" << texDesc.width << "TEX H:" << texDesc.height << std::endl;
                    texturePaint = gte.graphicsEngine->makeTexture(texDesc);
                    texturePaint->copyBytes((void *)_params->img->data,_params->img->header.stride);
                }

                te_params.addAttachment(OmegaGTE::TETessellationParams::Attachment::makeTexture2D(r.w,r.h));

                result = tessellationEngineContext->tessalateSync(te_params,OmegaGTE::GTEPolygonFrontFaceRotation::Clockwise,&viewPort);
                result.translate(-((viewPort.width/2) - _params->rect.pos.x),
                                 -((viewPort.height/2) - _params->rect.pos.y),
                                 0,
                                 viewPort);

                break;
            }
            case VisualCommand::RoundedRect : {
                auto _params = (VisualCommand::RoundedRectParams *)params;
                auto te_params = OmegaGTE::TETessellationParams::RoundedRect(_params->rect);

                useTextureRenderPipeline = !_params->brush->isColor;

                if(!useTextureRenderPipeline){
                    auto color = OmegaGTE::makeColor(_params->brush->color.r,
                                                     _params->brush->color.g,
                                                     _params->brush->color.b,
                                                     _params->brush->color.a);
                    te_params.addAttachment(OmegaGTE::TETessellationParams::Attachment::makeColor(color));
                }
                result = tessellationEngineContext->tessalateSync(te_params,OmegaGTE::GTEPolygonFrontFaceRotation::Clockwise,&viewPort);
                result.translate(-((viewPort.width/2) - _params->rect.pos.x),
                                 -((viewPort.height/2) - _params->rect.pos.y),
                                 0,
                                 viewPort);

                break;
            }
        }

        if(useTextureRenderPipeline){
            struct_size = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT4,OMEGASL_FLOAT2});
        }
        else {
            struct_size = OmegaGTE::omegaSLStructSize({OMEGASL_FLOAT4,OMEGASL_FLOAT4});
        }

        OmegaGTE::BufferDescriptor bufferDesc {OmegaGTE::BufferDescriptor::Upload,result.totalVertexCount() *struct_size,struct_size};
        auto buffer = gte.graphicsEngine->makeBuffer(bufferDesc);

        bufferWriter->setOutputBuffer(buffer);

        auto cb = preEffectTarget->commandBuffer();

        if(textureFence != nullptr){
            preEffectTarget->notifyCommandBuffer(cb,textureFence);
        }
        
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

        unsigned startVertexIndex = 0;

        auto writeColorVertexToBuffer = [&](OmegaGTE::GPoint3D & pt,OmegaGTE::FVec<4> & color){
            auto pos = OmegaGTE::FVec<4>::Create();
            pos[0][0] = pt.x;
            pos[1][0] = pt.y;
            pos[2][0] = pt.z;
            pos[3][0] = 1.f;
            bufferWriter->structBegin();
            bufferWriter->writeFloat4(pos);
            bufferWriter->writeFloat4(color);
            bufferWriter->structEnd();
            bufferWriter->sendToBuffer();
        };

         auto writeTexVertexToBuffer = [&](OmegaGTE::GPoint3D & pt,OmegaGTE::FVec<2> & coord){
            auto pos = OmegaGTE::FVec<4>::Create();
            pos[0][0] = pt.x;
            pos[1][0] = pt.y;
            pos[2][0] = pt.z;
            pos[3][0] = 1.f;
            bufferWriter->structBegin();
            bufferWriter->writeFloat4(pos);
            bufferWriter->writeFloat2(coord);
            bufferWriter->structEnd();
            bufferWriter->sendToBuffer();
        };


        for(auto & m : result.meshes) {
            for(auto & v : m.vertexPolygons){
                if(useTextureRenderPipeline){
                    writeTexVertexToBuffer(v.a.pt,v.a.attachment->texture2Dcoord);
                    writeTexVertexToBuffer(v.b.pt,v.b.attachment->texture2Dcoord);
                    writeTexVertexToBuffer(v.c.pt,v.c.attachment->texture2Dcoord);
                }
                else {
                    writeColorVertexToBuffer(v.a.pt,v.a.attachment->color);
                    writeColorVertexToBuffer(v.b.pt,v.b.attachment->color);
                    writeColorVertexToBuffer(v.c.pt,v.c.attachment->color);
                }
            }
        }

        cb->startRenderPass(renderPassDesc);
        if(useTextureRenderPipeline){
            cb->setRenderPipelineState(textureRenderPipelineState);
            cb->bindResourceAtVertexShader(buffer,1);
            cb->bindResourceAtFragmentShader(texturePaint,2);
        }
        else {
             cb->setRenderPipelineState(renderPipelineState);
             cb->bindResourceAtVertexShader(buffer,0);
        }
        cb->setViewports({viewport});
        cb->setScissorRects({scissorRect});


        for(auto & m : result.meshes){
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
        bufferWriter->flush();
        cb->endRenderPass();
        preEffectTarget->submitCommandBuffer(cb);
    }



}