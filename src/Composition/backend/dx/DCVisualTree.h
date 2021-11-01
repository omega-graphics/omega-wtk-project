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
    struct Visual : public Parent::Visual {
        IDXGISwapChain3 *swapChain;
        IDCompositionVisual2 * visual,*parent = nullptr;
        IDCompositionVisual2 * shadowVisual = nullptr;
        IDCompositionMatrixTransform3D *transformEffect = nullptr;
        IDCompositionShadowEffect *shadowEffect = nullptr;
        explicit Visual(Core::Position & pos,BackendRenderTargetContext &context, IDCompositionVisual2 * visual, IDXGISwapChain3 *swapChain);
        void updateShadowEffect(LayerEffect::DropShadowParams &params) override;
        void updateTransformEffect(LayerEffect::TransformationParams &params) override;
        void resize(Core::Rect &newRect) override;
        ~Visual() override;
    };
    explicit DCVisualTree(SharedHandle<ViewRenderTarget> & view);
    void addVisual(Core::SharedPtr<Parent::Visual> & visual) override;
    Core::SharedPtr<Parent::Visual> makeVisual(Core::Rect & rect,Core::Position & pos) override;
    void setRootVisual(Core::SharedPtr<Parent::Visual> & visual) override;
   
};

};

#endif