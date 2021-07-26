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
        using Parent::body;
        using Parent::root;
        /**
         A small structure that holds CAMetalLayer, CATransformLayer,
         as well a SharedHandle to the original BDCompositionImage
         */
        struct Visual : public Parent::Visual {
            CAMetalLayer *metalLayer;
            CATransformLayer *transformLayer;
            bool attachTransformLayer;
        };
    public:
        MTLCALayerTree();
        ~MTLCALayerTree();
        static Core::SharedPtr<MTLCALayerTree> Create();
        void addVisual(Core::SharedPtr<Parent::Visual> & visual) override;
        Core::SharedPtr<Parent::Visual> makeVisual(GERenderTargetContext & renderContext,
                                                   OmegaGTE::NativeRenderTargetDescriptor & targetDesc,
                                                   Core::Position & pos) override;
        void setRootVisual(Core::SharedPtr<Parent::Visual> & visual) override;
    };

    

};

#endif
