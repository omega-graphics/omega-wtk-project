#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H
#define OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H


namespace OmegaWTK::Composition {



    class GERenderTargetContext {
        
        OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> renderTarget;
        OmegaGTE::SharedHandle<OmegaGTE::OmegaTessalationEngineContext> tessContext;
        bool __hasQueuedRenderJobs;
    public:
        OmegaGTE::GERenderTarget *getRenderTarget();
        bool hasQueuedRenderJobs();
        void renderToTarget(VisualCommand::Type type,void *params);
        void applyEffectToTarget(CanvasLayerEffect::Type type,void *params);
        /**
         Commit all queued render jobs to GPU.
        */
        void commit();
        
        /**
            Create a GERenderTarget Context
            @param renderTarget
        */
        GERenderTargetContext(OmegaGTE::SharedHandle<OmegaGTE::GENativeRenderTarget> & renderTarget);
        ~GERenderTargetContext(); 
    };

    class BackendVisualTree;

    struct BackendCompRenderTarget {
        GERenderTargetContext * viewRenderTarget;
        Core::SharedPtr<BackendVisualTree> visualTree;
        OmegaCommon::MapVec<CanvasSurface *,GERenderTargetContext *> surfaceTargets;
        void renderVisualTree();
    };



    struct RenderTargetStore {
     private:
        void cleanTargets(LayerTree *tree,LayerTree::Limb *limb);
    public:
        void cleanTreeTargets(LayerTree *tree);
        OmegaCommon::MapVec<CompositionRenderTarget *,BackendCompRenderTarget> store = {};
    };

};

#endif