#include "../BDCompositionVisualTree.h"
#include "MTLBDCompositionDevice.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCALAYERTREE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCALAYERTREE_H

@class CALayer;
@class CAMetalLayer;
@class CATransformLayer;

namespace OmegaWTK::Composition {
    /**
     Metal Backend Impl of the BDCompositionVisualTree using CALayers
     */
    class MTLBDCALayerTree : public BDCompositionVisualTree {
        MTLBDCompositionDevice *device;
        typedef BDCompositionVisualTree Parent;
        friend class MTLBDCompositionDevice;
    public:
        /**
         A small structure that holds CAMetalLayer, CATransformLayer,
         as well a SharedHandle to the original BDCompositionImage
         */
        struct Visual : public Parent::Visual {
            CAMetalLayer *metalLayer;
            CATransformLayer *transformLayer;
            Core::SharedPtr<BDCompositionImage> img;
            bool attachTransformLayer;
        };
    public:
        MTLBDCALayerTree(MTLBDCompositionDevice *device);
        static Core::SharedPtr<MTLBDCALayerTree> Create(MTLBDCompositionDevice *device);
        Core::SharedPtr<Parent::Visual> makeVisual(Core::SharedPtr<BDCompositionImage> &img);
        void setRootVisual(Core::SharedPtr<Parent::Visual> &visual);
        void addVisual(Core::SharedPtr<Parent::Visual> &visual);
    };

};

#endif
