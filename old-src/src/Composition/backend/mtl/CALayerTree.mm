#include "../RenderTarget.h"
#import "CALayerTree.h"

#import <QuartzCore/QuartzCore.h>

#include "NativePrivate/macos/CocoaUtils.h"
#include "NativePrivate/macos/CocoaItem.h"



namespace OmegaWTK::Composition {

// Core::SharedPtr<BackendVisualTree> CreateVisualTree(){
//     return MTLCALayerTree::Create();
// };


// OmegaGTE::NativeRenderTargetDescriptor * makeDescForViewRenderTarget(
//                                         ViewRenderTarget *renderTarget){

//     auto cocoaView = (Native::Cocoa::CocoaItem *)renderTarget->getNativePtr();
//     auto *desc = new OmegaGTE::NativeRenderTargetDescriptor;
//     OmegaWTKCocoaView *view = (__bridge OmegaWTKCocoaView *)cocoaView->getBinding();
//     CAMetalLayer *metalLayer = [CAMetalLayer layer];
//     metalLayer.frame = view.frame;
//     metalLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
//     view.layer = metalLayer;
//     desc->metalLayer = metalLayer;
//     return desc;
// };

// OmegaGTE::NativeRenderTargetDescriptor * makeDescForCanvasSurface(CanvasSurface *layer){
//    auto *desc = new OmegaGTE::NativeRenderTargetDescriptor;
//    CAMetalLayer *metalLayer = [CAMetalLayer layer];
//    metalLayer.frame = Native::Cocoa::core_rect_to_cg_rect(layer->getParentLayer()->getLayerRect());
//    metalLayer.contentsScale = [NSScreen mainScreen].backingScaleFactor;
//    desc->metalLayer = metalLayer;
//    return desc;
// };

// MTLCALayerTree::MTLCALayerTree(){

// };

// Core::SharedPtr<MTLCALayerTree> MTLCALayerTree::Create(){
//     return Core::SharedPtr<MTLCALayerTree>(new MTLCALayerTree());
// };

// Core::SharedPtr<BackendVisualTree::Visual> MTLCALayerTree::makeVisual(
//                                                             GERenderTargetContext &renderContext,
//                                                             OmegaGTE::NativeRenderTargetDescriptor & targetDesc,
//                                                             Core::Position & pos){

//     auto * v = new Visual{{renderContext}};
//     v->attachTransformLayer = false;
//     v->metalLayer = targetDesc.metalLayer;
//     v->pos = pos;
//     return std::shared_ptr<Visual>(v);
// };

// void MTLCALayerTree::setRootVisual(Core::SharedPtr<BackendVisualTree::Visual> & visual){
//     root = visual;
// };

// void MTLCALayerTree::addVisual(Core::SharedPtr<Parent::Visual> & visual){
//     body.push_back(visual);
// };

// void BackendCompRenderTarget::renderVisualTree(){
//     MTLCALayerTree *layerTree = (MTLCALayerTree *)visualTree.get();
//     MTLCALayerTree::Visual *root = (MTLCALayerTree::Visual *)layerTree->root.get();
//     OmegaGTE::GENativeRenderTarget *target = (OmegaGTE::GENativeRenderTarget *)root->imgRenderTarget.getRenderTarget();
//     root->metalLayer.anchorPoint = CGPointMake(0.f,0.f);
//     target->commitAndPresent();
//     for(auto & child : layerTree->body){
//         CAMetalLayer *childLayer = ((MTLCALayerTree::Visual *)child.get())->metalLayer;
//         [root->metalLayer addSublayer:childLayer];
//         childLayer.position = Native::Cocoa::core_pos_to_cg_point(child->pos);
//         childLayer.anchorPoint = CGPointMake(0.f,0.f);
//         [childLayer setNeedsDisplay];
//     };
//     [root->metalLayer setNeedsDisplay];
// };

// void GERenderTargetContext::applyEffectToTarget(CanvasLayerEffect::Type type, void *params){
        
// };

// MTLCALayerTree::~MTLCALayerTree(){
    
// };

// Core::SharedPtr<MTLBDCALayerTree> MTLBDCALayerTree::Create(MTLBDCompositionDeviceContext *deviceContext){
//     return std::make_shared<MTLBDCALayerTree>(deviceContext);
// };

// Core::SharedPtr<BDCompositionVisualTree::Visual> MTLBDCALayerTree::makeVisual(Core::SharedPtr<BDCompositionImageRenderTarget> & imgTarget,Core::SharedPtr<BDCompositionImage> & img){
//     MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)img.get();
// //    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
// //    Core::Rect scaleTarget = Rect(mtlImg->n_rect.pos.x * scaleFactor,mtlImg->n_rect.pos.y * scaleFactor,mtlImg->n_rect.dimen.minWidth * scaleFactor,mtlImg->n_rect.dimen.minHeight * scaleFactor);
//     auto caLayerRenderTarget = deviceContext->makeCALayerRenderTarget(mtlImg->n_rect);
//     Color bkgrd {Composition::Color::Black,0x00};
//     caLayerRenderTarget->clear(bkgrd);
//     caLayerRenderTarget->drawImage(img,Core::FPosition({0.f,0.f}));
//     caLayerRenderTarget->commit();
//     CAMetalLayer *metalLayer = caLayerRenderTarget->metalLayer;
// //    metalLayer.anchorPoint = CGPointMake(0.0,0.0);
//     if(mtlImg->dropShadow){
//         LayerEffect::DropShadowParams *params = (LayerEffect::DropShadowParams *)mtlImg->dropShadow->params;
//         metalLayer.shadowColor = color_to_ns_color(params->color).CGColor;
//         metalLayer.shadowOpacity = params->opacity;
//         metalLayer.shadowRadius = params->radius;
//         metalLayer.shadowOffset = CGSizeMake(params->x_offset,params->y_offset);
//     };
//     NSLog(@"Creating Visual");
//     Visual visual;
//     visual.attachTransformLayer = false;
//     visual.img = img;
//     visual.metalLayer = metalLayer;
//     visual.pos = mtlImg->n_rect.pos;
//     visual.transformLayer = nullptr;
//     visual.imgTarget = imgTarget.get();
//     return std::make_shared<Visual>(std::move(visual));
// };

// Core::SharedPtr<BDCompositionVisualTree::Visual> MTLBDCALayerTree::makeVisual(BDCompositionImageRenderTarget * imgTarget,Core::SharedPtr<BDCompositionImage> & img){
//      MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)img.get();
// //    auto scaleFactor = [NSScreen mainScreen].backingScaleFactor;
// //    Core::Rect scaleTarget = Rect(mtlImg->n_rect.pos.x * scaleFactor,mtlImg->n_rect.pos.y * scaleFactor,mtlImg->n_rect.dimen.minWidth * scaleFactor,mtlImg->n_rect.dimen.minHeight * scaleFactor);
//     auto caLayerRenderTarget = deviceContext->makeCALayerRenderTarget(mtlImg->n_rect);
//     Color bkgrd {Composition::Color::Black,0x00};
//     caLayerRenderTarget->clear(bkgrd);
//     caLayerRenderTarget->drawImage(img,Core::FPosition({0.f,0.f}));
//     caLayerRenderTarget->commit();
//     CAMetalLayer *metalLayer = caLayerRenderTarget->metalLayer;
// //    metalLayer.anchorPoint = CGPointMake(0.0,0.0);
//     if(mtlImg->dropShadow){
//         LayerEffect::DropShadowParams *params = (LayerEffect::DropShadowParams *)mtlImg->dropShadow->params;
//         metalLayer.shadowColor = color_to_ns_color(params->color).CGColor;
//         metalLayer.shadowOpacity = params->opacity;
//         metalLayer.shadowRadius = params->radius;
//         metalLayer.shadowOffset = CGSizeMake(params->x_offset,params->y_offset);
//     };
//     NSLog(@"Creating Visual");
//     Visual visual;
//     visual.attachTransformLayer = false;
//     visual.img = img;
//     visual.metalLayer = metalLayer;
//     visual.pos = mtlImg->n_rect.pos;
//     visual.transformLayer = nullptr;
//     visual.imgTarget = imgTarget;
//     return std::make_shared<Visual>(std::move(visual));
// };

// void MTLBDCALayerTree::setRootVisual(Core::SharedPtr<BDCompositionVisualTree::Visual>  visual){
//     root_v = visual;
// };

// void MTLBDCALayerTree::replaceRootVisual(Core::SharedPtr<BDCompositionVisualTree::Visual> visual){
//     MTLBDCALayerTree::Visual *_n_v = (MTLBDCALayerTree::Visual *)visual.get();
//     MTLBDCALayerTree::Visual *r_v = (MTLBDCALayerTree::Visual *)root_v.get();
//     r_v->img = _n_v->img;
//     r_v->attachTransformLayer = _n_v->attachTransformLayer;
//     r_v->pos = _n_v->pos;
//     r_v->metalLayer = _n_v->metalLayer;
//     r_v->transformLayer = _n_v->transformLayer;
// };

// void MTLBDCALayerTree::replaceVisualWithTargetPtr(BDCompositionImageRenderTarget * imgTarget,Core::SharedPtr<BDCompositionVisualTree::Visual> visual){
//     MTLBDCALayerTree::Visual *_n_v = (MTLBDCALayerTree::Visual *)visual.get();
//     for(auto & v : body){
//         MTLBDCALayerTree::Visual *_v = (MTLBDCALayerTree::Visual *)v.get();
//         if(_v->imgTarget == imgTarget){
//             _v->img = _n_v->img;
//             _v->attachTransformLayer = _n_v->attachTransformLayer;
//             _v->pos = _n_v->pos;
//             _v->metalLayer = _n_v->metalLayer;
//             _v->transformLayer = _n_v->transformLayer;
//             break;
//             return;
//         };
//     };
// };

// void MTLBDCALayerTree::addVisual(Core::SharedPtr<BDCompositionVisualTree::Visual> & visual){
//     body.push_back(visual);
// };

// void MTLBDCALayerTree::drawNewCompImageToVisual(BDCompositionImageRenderTarget *imgTarget, Core::SharedPtr<BDCompositionImage> &img){
//     MTLBDCALayerTree::Visual *r_v = (MTLBDCALayerTree::Visual *)root_v.get();
//     if(r_v->imgTarget == imgTarget){
//         r_v->img = img;
//         MTLBDCompositionImage *__img = (MTLBDCompositionImage *)r_v->img.get();
//         r_v->pos = __img->n_rect.pos;
//         auto target = deviceContext->makeCALayerRenderTarget(r_v->metalLayer,__img->n_rect);
//         Color bkgrd {Composition::Color::Black,0x00};
//         target->clear(bkgrd);
//         target->drawImage(img,Core::FPosition({0.f,0.f}));
//         target->commit();
//         return;
//     };

//     for(auto & v : body){
//         MTLBDCALayerTree::Visual *_v = (MTLBDCALayerTree::Visual *)v.get();
//         if(_v->imgTarget == imgTarget){
//             _v->img = img;
//             MTLBDCompositionImage *__img = (MTLBDCompositionImage *)_v->img.get();
//             _v->pos = __img->n_rect.pos;
//             auto target = deviceContext->makeCALayerRenderTarget(_v->metalLayer,__img->n_rect);
//             Color bkgrd {Composition::Color::Black,0x00};
//             target->clear(bkgrd);
//             target->drawImage(img,Core::FPosition({0.f,0.f}));
//             target->commit();
//             break;
//         };
//     };
// };

// void MTLBDCALayerTree::layout(){
//     CGFloat scaleFactor = [NSScreen mainScreen].backingScaleFactor;
//     Visual *rootV = (Visual *)root_v.get();
//     MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)rootV->img.get();
//     CALayer *superLayer = rootV->metalLayer.superlayer;
//     rootV->metalLayer.frame = CGRectMake(rootV->pos.x,rootV->pos.y,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
//     rootV->metalLayer.bounds = CGRectMake(0.0,0.0,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
//     rootV->metalLayer.position = CGPointMake(rootV->pos.x - (superLayer.position.x * scaleFactor),rootV->pos.y - (superLayer.position.y * scaleFactor));
//     auto visual_it = body.begin();
//     while(visual_it != body.end()){
//         auto _v = (MTLBDCALayerTree::Visual *)visual_it->get();
//         MTLBDCompositionImage *mtlImg = (MTLBDCompositionImage *)_v->img.get();
//         _v->metalLayer.frame = CGRectMake(_v->pos.x,_v->pos.y,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
//         _v->metalLayer.bounds = CGRectMake(0.0,0.0,mtlImg->n_rect.dimen.minWidth,mtlImg->n_rect.dimen.minHeight);
//         _v->metalLayer.position = CGPointMake(_v->pos.x,_v->pos.y);
// //        NSLog(@"View Layer's Pos: {x:%f ,y:%f}",root->metalLayer.position.x,root->metalLayer.position.y);
//         NSLog(@"Metal Layer's Pos: {x:%f ,y:%f}",_v->metalLayer.position.x,_v->metalLayer.position.y);
//         NSLog(@"Layer Rect: {x:%f,y:%f,w:%f,h:%f",_v->metalLayer.bounds.origin.x,_v->metalLayer.bounds.origin.y,_v->metalLayer.bounds.size.width,_v->metalLayer.bounds.size.height);
//         [_v->metalLayer setNeedsDisplay];
// //        [_v->metalLayer layoutIfNeeded];
//         NSLog(@"SuperLayer: %@",_v->metalLayer.superlayer);
//         ++visual_it;
//     };
//     [rootV->metalLayer setNeedsDisplay];
// };

};
