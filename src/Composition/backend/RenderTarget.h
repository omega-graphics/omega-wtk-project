#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/CompositorClient.h"

#ifndef OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H
#define OMEGAWTK_COMPOSITION_BACKED_RENDERTARGETSTORE_H


namespace OmegaWTK::Composition {



    class GERenderTargetContext {
        
        OmegaGTE::SharedHandle<OmegaGTE::GERenderTarget> renderTarget;


    public:
        void renderToTarget(VisualCommand::Type type,void *params);
        void commit();
        
        /**
        Create a GERenderTarget Context
        */
        GERenderTargetContext(OmegaGTE::SharedHandle<OmegaGTE::GERenderTarget> renderTarget);
    };

    struct BackendCompRenderTarget {
        GERenderTargetContext viewRenderTarget;
        OmegaCommon::MapVec<CanvasSurface *,GERenderTargetContext> surfaceTargets;
    };



    struct RenderTargetStore {
        OmegaCommon::MapVec<CompositionRenderTarget *,BackendCompRenderTarget> store;
    };

};

#endif