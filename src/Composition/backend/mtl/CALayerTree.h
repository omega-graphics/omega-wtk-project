#include "../VisualTree.h"
#include "../RenderTarget.h"

#ifndef OMEGAWTK_COMPOSITION_MTL_MTLBDCALAYERTREE_H
#define OMEGAWTK_COMPOSITION_MTL_MTLBDCALAYERTREE_H

// #ifdef __OBJC__

#include "NativePrivate/macos/CocoaItem.h"
#import <QuartzCore/QuartzCore.h>


// #else 

// struct CALayer;
// struct CAMetalLayer;
// struct CATransformLayer;

// #endif

namespace OmegaWTK::Composition {
    // /**
    //  Metal Backend Impl of the BDCompositionVisualTree using CALayers
    //  */
     class MTLCALayerTree : public BackendVisualTree {
         typedef BackendVisualTree Parent;
         SharedHandle<Native::Cocoa::CocoaItem> view;
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
             explicit Visual(Core::Position & pos,
                    BackendRenderTargetContext &renderTarget,
                    CAMetalLayer *metalLayer,
                    CATransformLayer *transformLayer,
                    bool attachTransformLayer):
                     Parent::Visual(pos,renderTarget),
                     metalLayer(metalLayer),
                     transformLayer(transformLayer),
                     attachTransformLayer(attachTransformLayer){

             };
             void resize(Core::Rect & newRect) override {
                 [metalLayer setFrame:CGRectMake(newRect.pos.x,newRect.pos.y,newRect.w,newRect.h)];
             }
             ~Visual() override = default;
         };
     public:
         explicit MTLCALayerTree(SharedHandle<ViewRenderTarget> & renderTarget);
         ~MTLCALayerTree() override = default;
         void addVisual(Core::SharedPtr<Parent::Visual> & visual) override;
         Core::SharedPtr<Parent::Visual> makeVisual(Core::Rect & rect,
                                                    Core::Position & pos) override;
         void setRootVisual(Core::SharedPtr<Parent::Visual> & visual) override;
     };

    

};

#endif
