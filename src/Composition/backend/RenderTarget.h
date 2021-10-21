#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Canvas.h"

#ifndef OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H
#define OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H


namespace OmegaWTK::Composition {

    class BackendRenderTargetContext {
        OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> renderTarget;
        OmegaGTE::SharedHandle<OmegaGTE::OmegaTessellationEngineContext> tessellationEngineContext;
        Core::Rect & renderTargetSize;
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
        OmegaCommon::MapVec<Layer *,BackendRenderTargetContext *> surfaceTargets;
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