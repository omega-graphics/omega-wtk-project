#include "../RenderTarget.h"
#include "../VisualTree.h"

#include <dcomp.h>

#pragma comment(lib,"dcomp.lib")


#ifndef OMEGAWTK_COMPOSITION_DX_DCVISUALTREE_H
#define OMEGAWTK_COMPOSITION_DX_DCVISUALTREE_H

namespace OmegaWTK::Composition {
/**
     DirectX Backend Impl of the BDCompositionVisualTree using IDCompositionVisuals
*/
class DCVisualTree : public BackendVisualTree {
    Core::UniqueComPtr<IDCompositionTarget> hwndTarget;
    typedef BackendVisualTree Parent;
    public:
    struct Visual : Parent::Visual {
        IDCompositionVisual2 * visual;
        IDCompositionVisual2 * shadowVisual = nullptr;
        // OmegaGTE::SharedHandle<OmegaGTE::GETextureRenderTarget> img;
        Core::Position pos;
        ~Visual();
    };
    DCVisualTree();
    static Core::SharedPtr<BackendVisualTree> Create();
    void addVisual(Core::SharedPtr<Parent::Visual> & visual) override;
    Core::SharedPtr<Parent::Visual> makeVisual(GERenderTargetContext & renderContext,
                                                            OmegaGTE::NativeRenderTargetDescriptor & targetDesc,
                                                            Core::Position & pos) override;
    void setRootVisual(Core::SharedPtr<Parent::Visual> & visual) override;
   
};

};

#endif