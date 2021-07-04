#include "../VisualTree.h"
#include "../RenderTarget.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCALAYERTREE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCALAYERTREE_H

// #ifdef __OBJC__

@class CALayer;
@class CAMetalLayer;
@class CATransformLayer;

// #else 

// struct CALayer;
// struct CAMetalLayer;
// struct CATransformLayer;

// #endif

namespace OmegaWTK::Composition {
    /**
     Metal Backend Impl of the BDCompositionVisualTree using CALayers
     */
    class MTLCALayerTree : public BackendVisualTree {
        // MTLBDCompositionDeviceContext *deviceContext;
        typedef BackendVisualTree Parent;
        friend class MTLBDCompositionDeviceContext;
    public:
        /**
         A small structure that holds CAMetalLayer, CATransformLayer,
         as well a SharedHandle to the original BDCompositionImage
         */
        struct Visual : public Parent::Visual {
            CAMetalLayer *metalLayer;
            CATransformLayer *transformLayer;
            GERenderTargetContext imgRenderTaget;
            Core::Position pos;
            bool attachTransformLayer;
        };
    public:
        MTLCALayerTree();
        static Core::SharedPtr<MTLCALayerTree> Create();
        Core::SharedPtr<Parent::Visual> makeVisual(Core::SharedPtr<BDCompositionImageRenderTarget> & imgTarget,Core::SharedPtr<BDCompositionImage> &img);
        Core::SharedPtr<Parent::Visual> makeVisual(BDCompositionImageRenderTarget *imgTarget,Core::SharedPtr<BDCompositionImage> &img);
        void setRootVisual(Core::SharedPtr<Parent::Visual> & visual);
        void drawNewCompImageToVisual(BDCompositionImageRenderTarget *imgTarget, Core::SharedPtr<BDCompositionImage> &img);
        void replaceRootVisual(Core::SharedPtr<Parent::Visual> visual);
        void replaceVisualWithTargetPtr(BDCompositionImageRenderTarget * imgTarget, Core::SharedPtr<Parent::Visual> visual);
        void addVisual(Core::SharedPtr<Parent::Visual> & visual);
        void layout();
    };

    

};

#endif
