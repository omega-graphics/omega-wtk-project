#include "BackendPriv.h"
#include "BDCompositionRenderTarget.h"
#include "BDCompositionFontFactory.h"
#include "BDCompositionVisualTree.h"

#include <iostream>

namespace OmegaWTK::Composition {
    Core::SharedPtr<BDCompositionDevice> global_device;
    Core::SharedPtr<BDCompositionFontFactory> global_font_factory;
    bool hasGlobalDevice = false;

    BackendImpl::BackendImpl(){
        // MessageBoxA(GetForegroundWindow(),"Setting up Backend Private!","NOTE",MB_OK);
        if(!hasGlobalDevice){
            global_device = BDCompositionDevice::Create();
            hasGlobalDevice = true;
            global_font_factory = global_device->createFontFactory();
        }
    };
    /**
     Draws basic visuals onto a render target
     */
    void BackendImpl::drawVisual(BDCompositionRenderTarget * target,Visual *visual,bool updatePass){
        // MessageBoxA(GetForegroundWindow(),"Will Draw Rect","NOTE",MB_OK);
        switch (visual->type) {
        case Visual::Rect : {
            Visual::RectParams *params = (Visual::RectParams *)visual->params;
            target->fillRect(params->rect,params->brush);
            if(params->border.has_value()){
                target->frameRect(params->rect,params->border->brush,params->border->width);
            };
            break;
        };
        case Visual::RoundedRect : {
            Visual::RoundedRectParams *params = (Visual::RoundedRectParams *)visual->params;
            auto shape = Core::RoundedRect(params->rect,params->rad_x,params->rad_y);
            target->fillRoundedRect(shape,params->brush);
            if(params->border.has_value()){
                target->frameRoundedRect(shape,params->border->brush,params->border->width);
            };
            break;
        }
        case Visual::Bitmap : {
            Visual::BitmapParams *params = (Visual::BitmapParams *)visual->params;
            if(updatePass){
                auto & img = target->getImage(visual->id);
                #ifdef TARGET_WIN32
                if(target->targetIsFresh())
                    img->redoImage();
                #endif
                target->drawImage(img,params->rect.pos);
            }
            else {
                auto img_ptr = target->createImageFromBitmapImage(params->img,params->rect,visual->id);
                target->drawImage(img_ptr,params->rect.pos);
            };
            break;
        };
        case Visual::Text : {
            Visual::TextParams *params = (Visual::TextParams *)visual->params;
            // if(updatePass){

            // }
            // else{
            auto font = global_font_factory->createFont(params->text.getFont(),params->text.getFontSize());
            target->drawText(font,params->text.getString(),params->rect,params->brush);
            // };
            break;
        };
        default: 
            break;
        }
    };
    void BackendImpl::doWork(){
        auto visualTree = global_device->createVisualTree();
        
        Layer * rootLayer = currentLimb->limbRoot;
        auto rootImgTarget = global_device->makeImageRenderTarget(rootLayer->getLayerRect());
        rootImgTarget->clear(rootLayer->style->background);
        auto __visual_it = rootLayer->style->visuals.begin();
        while(__visual_it != rootLayer->style->visuals.end()){
            drawVisual(rootImgTarget.get(),__visual_it->get(),false);
            ++__visual_it;
        };
        rootImgTarget->commit();
        Core::SharedPtr<BDCompositionImage> rootImg = rootImgTarget->getImg();
        auto visual = visualTree->makeVisual(rootImg);
        visualTree->setRootVisual(visual);
        layerTargets.insert(std::make_pair(rootLayer,rootImgTarget));
        {
            /// TODO: Eventually Change to Branch Iteration Model!
            auto it = currentLimb->begin();
            while(it != currentLimb->end()){
                Layer *layer = it->get();
                auto imgTarget = global_device->makeImageRenderTarget(layer->getLayerRect());
                imgTarget->clear(layer->style->background);
                auto __visual_it = layer->style->visuals.begin();
                while(__visual_it != layer->style->visuals.end()){
                    drawVisual(imgTarget.get(),__visual_it->get(),false);
                    ++__visual_it;
                };
                imgTarget->commit();
                Core::SharedPtr<BDCompositionImage> resultImg = imgTarget->getImg();
                auto visual = visualTree->makeVisual(resultImg);
                visualTree->addVisual(visual);
                layerTargets.insert(std::make_pair(layer,imgTarget));
                ++it;
            };
        }
        
        global_device->renderVisualTreeToView(visualTree,currentLimb->renderTarget);
        visualTrees.insert(std::make_pair(currentLimb->renderTarget,visualTree));
    };
    void BackendImpl::doUpdate(){
//        auto & target = targets[currentLayer];
//
//        #ifdef TARGET_WIN32
//        if(target->needsSwapChain()){
//            target->redoSwapChain();
//        }
//        else if(target->needsDeviceContext()){
//            target->redoDeviceContext();
//        };
//        #endif
//
//        target->clear(currentLayer->getBackgroundColor());
//        auto & style = currentLayer->getStyle();
//        for(auto & visual : style.visuals){
//            drawVisual(target.get(),visual.get());
//        };
//        target->commit();
    };
};
