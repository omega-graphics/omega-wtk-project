#import "MTLBDCALayerTree.h"
#import "MTLBDCompositionImage.h"
#import "MTLBDTriangulator.h"
#import "MTLBDCompositionRenderTarget.h"

#import <QuartzCore/QuartzCore.h>

#include "NativePrivate/macos/CocoaUtils.h"

namespace OmegaWTK::Composition {
MTLBDCALayerTree::MTLBDCALayerTree(MTLBDCompositionDevice *device):device(device){
    
};

Core::SharedPtr<MTLBDCALayerTree> MTLBDCALayerTree::Create(MTLBDCompositionDevice *device){
    return std::make_shared<MTLBDCALayerTree>(device);
};

Core::SharedPtr<BDCompositionVisualTree::Visual> MTLBDCALayerTree::makeVisual(Core::SharedPtr<BDCompositionImage> & img){
    MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)img.get();
    auto caLayerRenderTarget = device->makeCALayerRenderTarget(mtlImg->n_rect);
    Color bkgrd {Composition::Color::Black,0x00};
    caLayerRenderTarget->clear(bkgrd);
    caLayerRenderTarget->drawImage(img,{0,0});
    caLayerRenderTarget->commit();
    CAMetalLayer *metalLayer = caLayerRenderTarget->metalLayer;
    Visual visual;
    visual.attachTransformLayer = false;
    visual.img = img;
    visual.metalLayer = metalLayer;
    visual.transformLayer = nullptr;
    return std::make_shared<Visual>(visual);
};

void MTLBDCALayerTree::setRootVisual(Core::SharedPtr<BDCompositionVisualTree::Visual> & visual){
    root_v = visual;
};

void MTLBDCALayerTree::addVisual(Core::SharedPtr<BDCompositionVisualTree::Visual> & visual){
    Visual *root = (Visual *)root_v.get();
    CALayer *rootCALayer = root->attachTransformLayer? root->metalLayer : root->transformLayer;
    Visual *child = (Visual *)visual.get();
    CALayer *childCALayer = child->attachTransformLayer? child->transformLayer : child->metalLayer;
    [rootCALayer addSublayer:childCALayer];
    body.push_back(visual);
};

};