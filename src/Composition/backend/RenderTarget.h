#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Canvas.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_RENDERTARGETSTORE_H
#define OMEGAWTK_COMPOSITION_BACKEND_RENDERTARGETSTORE_H


namespace OmegaWTK::Composition {

    INTERFACE BackendCanvasEffectProcessor {
    public:
        explicit BackendCanvasEffectProcessor(OmegaGTE::SharedHandle<OmegaGTE::GEFence> & fence):fence(fence){

        };
        OmegaGTE::SharedHandle<OmegaGTE::GEFence> fence;
      INTERFACE_METHOD void applyEffects(OmegaGTE::SharedHandle<OmegaGTE::GETexture> & dest,SharedHandle<OmegaGTE::GETextureRenderTarget> & textureTarget,OmegaCommon::Vector<std::pair<CanvasEffect::Type,void *>> & effects) ABSTRACT;
      static SharedHandle<BackendCanvasEffectProcessor> Create(OmegaGTE::SharedHandle<OmegaGTE::GEFence> & fence);
      virtual ~BackendCanvasEffectProcessor() = default;
    };

    class BackendRenderTargetContext {
        OmegaGTE::SharedHandle<OmegaGTE::GETexture> targetTexture;
        OmegaGTE::SharedHandle<OmegaGTE::GEFence> fence;
        OmegaGTE::SharedHandle<OmegaGTE::GETextureRenderTarget> preEffectTarget;
        OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> renderTarget;
        OmegaGTE::SharedHandle<OmegaGTE::OmegaTessellationEngineContext> tessellationEngineContext;
        SharedHandle<BackendCanvasEffectProcessor> imageProcessor;
        Core::Rect & renderTargetSize;
        OmegaCommon::Vector<std::pair<CanvasEffect::Type,void *>> effectQueue;
        void createGradientTexture(bool linearOrRadial,Gradient & gradient,OmegaGTE::GRect & rect,OmegaGTE::SharedHandle<OmegaGTE::GETexture> & dest);
    public:
        void clear(float r,float g,float b,float a);
        void renderToTarget(VisualCommand::Type type,void *params);
        void applyEffectToTarget(CanvasEffect::Type type, void *params);
        void setRenderTargetSize(Core::Rect &rect);
        /**
         Commit all queued render jobs to GPU.
        */
        void commit();
    
        /**
            Create a BackendRenderTarget Context
            @param renderTarget
        */
        explicit BackendRenderTargetContext(Core::Rect & rect,OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> & renderTarget);
    };

    class BackendVisualTree;



    struct BackendCompRenderTarget {
        SharedHandle<BackendVisualTree> visualTree;
        OmegaCommon::Map<Layer *,BackendRenderTargetContext *> surfaceTargets;
    };



    struct RenderTargetStore {
     private:
        void cleanTargets(LayerTree *tree,LayerTree::Limb *limb);
    public:
        void cleanTreeTargets(LayerTree *tree);
        OmegaCommon::Map<SharedHandle<CompositionRenderTarget>,BackendCompRenderTarget> store = {};
    };

};

#endif