#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H
#define OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H


namespace OmegaWTK::Composition {

    INTERFACE BackendRenderTargetContext {
    public:
        INTERFACE_METHOD void renderToTarget(VisualCommand::Type type,void *params) ABSTRACT;
        INTERFACE_METHOD void applyEffectToTarget(CanvasLayerEffect::Type type,void *params) ABSTRACT;
        /**
         Commit all queued render jobs to GPU.
        */
        INTERFACE_METHOD void commit() ABSTRACT;
    
        /**
            Create a GERenderTarget Context
            @param renderTarget
        */
    };

    class BackendVisualTree;



    struct BackendCompRenderTarget {
        BackendRenderTargetContext * viewRenderTarget;
        SharedHandle<BackendVisualTree> visualTree;
        OmegaCommon::MapVec<Canvas *,BackendRenderTargetContext *> surfaceTargets;
        // void renderVisualTree();
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