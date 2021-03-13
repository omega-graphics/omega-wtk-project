#import "MTLBDCALayerTree.h"
#import "MTLBDCompositionImage.h"
#import "MTLBDTriangulator.h"
#import "MTLBDCompositionRenderTarget.h"

#import <QuartzCore/QuartzCore.h>

#include "NativePrivate/macos/CocoaUtils.h"

namespace OmegaWTK::Composition {
MTLBDCALayerTree::MTLBDCALayerTree(MTLBDCompositionDeviceContext *deviceContext):deviceContext(deviceContext){
    
};

Core::SharedPtr<MTLBDCALayerTree> MTLBDCALayerTree::Create(MTLBDCompositionDeviceContext *deviceContext){
    return std::make_shared<MTLBDCALayerTree>(deviceContext);
};

Core::SharedPtr<BDCompositionVisualTree::Visual> MTLBDCALayerTree::makeVisual(Core::SharedPtr<BDCompositionImage> & img){
    MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)img.get();
//    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
//    Core::Rect scaleTarget = Rect(mtlImg->n_rect.pos.x * scaleFactor,mtlImg->n_rect.pos.y * scaleFactor,mtlImg->n_rect.dimen.minWidth * scaleFactor,mtlImg->n_rect.dimen.minHeight * scaleFactor);
    auto caLayerRenderTarget = deviceContext->makeCALayerRenderTarget(mtlImg->n_rect);
    Color bkgrd {Composition::Color::Black,0x00};
    caLayerRenderTarget->clear(bkgrd);
    caLayerRenderTarget->drawImage(img,Core::FPosition({0.f,0.f}));
    caLayerRenderTarget->commit();
    CAMetalLayer *metalLayer = caLayerRenderTarget->metalLayer;
//    metalLayer.anchorPoint = CGPointMake(0.0,0.0);
    if(mtlImg->dropShadow){
        LayerEffect::DropShadowParams *params = (LayerEffect::DropShadowParams *)mtlImg->dropShadow->params;
        metalLayer.shadowColor = color_to_ns_color(params->color).CGColor;
        metalLayer.shadowOpacity = params->opacity;
        metalLayer.shadowRadius = params->radius;
        metalLayer.shadowOffset = CGSizeMake(params->x_offset,params->y_offset);
    };
    NSLog(@"Creating Visual");
    Visual visual;
    visual.attachTransformLayer = false;
    visual.img = img;
    visual.metalLayer = metalLayer;
    visual.pos = mtlImg->n_rect.pos;
    visual.transformLayer = nullptr;
    return std::make_shared<Visual>(std::move(visual));
};

void MTLBDCALayerTree::setRootVisual(Core::SharedPtr<BDCompositionVisualTree::Visual>  visual){
    root_v = visual;
};

void MTLBDCALayerTree::replaceRootVisual(Core::SharedPtr<BDCompositionVisualTree::Visual> visual){
    
};

void MTLBDCALayerTree::replaceVisualWithTargetPtr(Core::SharedPtr<BDCompositionImageRenderTarget> & imgTarget,Core::SharedPtr<BDCompositionVisualTree::Visual> visual){
    
};

void MTLBDCALayerTree::addVisual(Core::SharedPtr<BDCompositionVisualTree::Visual> & visual){
    body.push_back(visual);
};

void MTLBDCALayerTree::layout(){
    CGFloat scaleFactor = [NSScreen mainScreen].backingScaleFactor;
    Visual *rootV = (Visual *)root_v.get();
    MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)rootV->img.get();
    CALayer *superLayer = rootV->metalLayer.superlayer;
    rootV->metalLayer.frame = CGRectMake(rootV->pos.x,rootV->pos.y,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
    rootV->metalLayer.bounds = CGRectMake(0.0,0.0,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
    rootV->metalLayer.position = CGPointMake(rootV->pos.x - (superLayer.position.x * scaleFactor),rootV->pos.y - (superLayer.position.y * scaleFactor));
    auto visual_it = body.begin();
    while(visual_it != body.end()){
        auto _v = (MTLBDCALayerTree::Visual *)visual_it->get();
        MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)_v->img.get();
        _v->metalLayer.frame = CGRectMake(_v->pos.x,_v->pos.y,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
        _v->metalLayer.bounds = CGRectMake(0.0,0.0,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
        _v->metalLayer.position = CGPointMake(_v->pos.x,_v->pos.y);
//        NSLog(@"View Layer's Pos: {x:%f ,y:%f}",root->metalLayer.position.x,root->metalLayer.position.y);
        NSLog(@"Metal Layer's Pos: {x:%f ,y:%f}",_v->metalLayer.position.x,_v->metalLayer.position.y);
        NSLog(@"Layer Rect: {x:%f,y:%f,w:%f,h:%f",_v->metalLayer.bounds.origin.x,_v->metalLayer.bounds.origin.y,_v->metalLayer.bounds.size.width,_v->metalLayer.bounds.size.height);
        [_v->metalLayer setNeedsDisplay];
//        [_v->metalLayer layoutIfNeeded];
        NSLog(@"SuperLayer: %@",_v->metalLayer.superlayer);
        ++visual_it;
    };
    [rootV->metalLayer setNeedsDisplay];
};

};
