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
        // MessageBoxA(HWND_DESKTOP,"Do Work","NOTE",MB_OK);
        Layer * rootLayer = currentLimb->limbRoot;
        auto rootImgTarget = global_device->makeImageRenderTarget(rootLayer->getLayerRect());
        // MessageBoxA(HWND_DESKTOP,"Created Image Target","NOTE",MB_OK);
        rootImgTarget->clear(rootLayer->style->background);
        // MessageBoxA(HWND_DESKTOP,"Cleared the Screen","NOTE",MB_OK);
        auto __visual_it = rootLayer->style->visuals.begin();
        while(__visual_it != rootLayer->style->visuals.end()){
            drawVisual(rootImgTarget.get(),__visual_it->get(),false);
            ++__visual_it;
        };
        rootImgTarget->commit();
        // MessageBoxA(HWND_DESKTOP,"Root Img Target Commit","NOTE",MB_OK);
    #if defined(TARGET_WIN32)
        auto visual = visualTree->makeVisual(rootImgTarget);
    #else
        Core::SharedPtr<BDCompositionImage> rootImg = rootImgTarget->getImg();
        auto effect_it = rootLayer->style->effects.begin();
        while(effect_it != rootLayer->style->effects.end()){
            rootImg->applyEffect(effect_it->get());
            ++effect_it;
        };
        auto visual = visualTree->makeVisual(rootImg);
    #endif
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
         #if defined(TARGET_WIN32)
                 auto _visual = visualTree->makeVisual(imgTarget);
         #else
                 Core::SharedPtr<BDCompositionImage> img = imgTarget->getImg();
                 auto effect_it = layer->style->effects.begin();
                 while(effect_it !=layer->style->effects.end()){
                     img->applyEffect(effect_it->get());
                     ++effect_it;
                 };
                 auto _visual = visualTree->makeVisual(img);
         #endif
                 visualTree->addVisual(_visual);
                 layerTargets.insert(std::make_pair(layer,imgTarget));
                 ++it;
             };
         }

        // MessageBoxA(HWND_DESKTOP,"Will Render Visual Tree to HWND","NOTE",MB_OK);
        
        global_device->renderVisualTreeToView(visualTree,currentLimb->renderTarget,false);
        visualTrees.insert(std::make_pair(currentLimb->renderTarget,visualTree));
    };
    void BackendImpl::doUpdate(){
        // MessageBoxA(HWND_DESKTOP,"Do Update","NOTE",MB_OK);
        auto & tree = visualTrees[currentLimb->renderTarget];
        auto & rootTarget = layerTargets[currentLimb->limbRoot];
        auto & rootLayer = currentLimb->limbRoot;
    #if defined(TARGET_WIN32)
        if(rootTarget->needsSwapChain()){
            rootTarget->redoSwapChain();
            auto r_visual = tree->makeVisual(rootTarget);
            tree->replaceRootVisual(r_visual);
        }
        else if(rootTarget->needsDeviceContext()){
            rootTarget->redoDeviceContext();

        };
    #endif
        /// Redraw Visuals!
        rootTarget->clear(rootLayer->style->background);
        auto __visual_it = rootLayer->style->visuals.begin();
        while(__visual_it != rootLayer->style->visuals.end()){
            drawVisual(rootTarget.get(),__visual_it->get(),true);
            ++__visual_it;
        };
        rootTarget->commit();

        auto v_it = currentLimb->begin();
        while(v_it != currentLimb->end()){
            auto child = v_it->get();
            auto & imgTarget = layerTargets[child];
    #if defined(TARGET_WIN32)
        if(imgTarget->needsSwapChain()){
            imgTarget->redoSwapChain();
            auto _visual = tree->makeVisual(imgTarget);
            tree->addVisual(_visual);
            tree->replaceVisualWithTargetPtr(imgTarget,_visual);
        }
        else if(imgTarget->needsDeviceContext()){
            imgTarget->redoDeviceContext();

        };
    #endif
        /// Redraw Visuals!
            imgTarget->clear(child->style->background);
            auto __visual_it = child->style->visuals.begin();
            while(__visual_it != child->style->visuals.end()){
                drawVisual(imgTarget.get(),__visual_it->get(),true);
                ++__visual_it;
            };
            rootTarget->commit();
            ++v_it;
        };

        global_device->renderVisualTreeToView(tree,currentLimb->renderTarget,true);
    };

    void BackendImpl::redoLayout(){
        auto & tree = visualTrees[currentLimb->renderTarget];
        tree->layout();
    };

};
