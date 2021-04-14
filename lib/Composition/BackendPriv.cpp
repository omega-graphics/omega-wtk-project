#include "BackendPriv.h"
#include "BDCompositionRenderTarget.h"
#include "BDCompositionFontFactory.h"
#include "BDCompositionVisualTree.h"

#include <iostream>

namespace OmegaWTK::Composition {
    Core::SharedPtr<BDCompositionDevice> global_device;
//    Core::SharedPtr<BDCompositionFontFactory> global_font_factory;
    bool hasGlobalDevice = false;

    BackendImpl::BackendImpl(){
        // MessageBoxA(GetForegroundWindow(),"Setting up Backend Private!","NOTE",MB_OK);
        if(!hasGlobalDevice){
            global_device = BDCompositionDevice::Create();
            hasGlobalDevice = true;
//            global_font_factory = global_device->createFontFactory();
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
            std::cout << "Will Draw Text" << std::endl;
            target->drawText(params->textRect,params->brush);
            
            break;
        };
        default: 
            break;
        }
    };
    void BackendImpl::doWork(){
        auto context = global_device->createContext();
        auto visualTree = context->createVisualTree();
        // MessageBoxA(HWND_DESKTOP,"Do Work","NOTE",MB_OK);
        Layer * rootLayer = currentLimb->limbRoot;
        auto rootImgTarget = context->makeImageRenderTarget(rootLayer->getLayerRect());
        // MessageBoxA(HWND_DESKTOP,"Created Image Target","NOTE",MB_OK);
        rootImgTarget->clear(rootLayer->style->background);
        // MessageBoxA(HWND_DESKTOP,"Cleared the Screen","NOTE",MB_OK);
        auto __visual_it = rootLayer->style->visuals.begin();
        while(__visual_it != rootLayer->style->visuals.end()){
            drawVisual(rootImgTarget.get(),*__visual_it,false);
            ++__visual_it;
        };
    #if !defined(TARGET_WIN32)
        rootImgTarget->commit();
    #endif
        // MessageBoxA(HWND_DESKTOP,"Root Img Target Commit","NOTE",MB_OK);
    #if defined(TARGET_WIN32)
        auto effect_it = rootLayer->style->effects.begin();
        while(effect_it !=rootLayer->style->effects.end()){
            rootImgTarget->applyEffect(effect_it->get());
            ++effect_it;
        };
        rootImgTarget->commitEffects();  
        rootImgTarget->commit();
        auto visual = visualTree->makeVisual(rootImgTarget);
    #else
        Core::SharedPtr<BDCompositionImage> rootImg = rootImgTarget->getImg();
        auto effect_it = rootLayer->style->effects.begin();
        while(effect_it != rootLayer->style->effects.end()){
            rootImg->applyEffect(effect_it->get());
            ++effect_it;
        };
        rootImg->commitEffects();
        auto visual = visualTree->makeVisual(rootImg);
    #endif
        std::cout << "Visual has been Created... Mounting root" << std::endl;
        visualTree->setRootVisual(visual);
        context->assignRenderTargetToLayer(rootLayer,rootImgTarget);
         {
             /// TODO: Eventually Change to Branch Iteration Model!
             auto it = currentLimb->begin();
             while(it != currentLimb->end()){
                 Layer *layer = it->get();
                 auto imgTarget = context->makeImageRenderTarget(layer->getLayerRect());
                 imgTarget->clear(layer->style->background);
                 auto __visual_it = layer->style->visuals.begin();
                 while(__visual_it != layer->style->visuals.end()){
                     drawVisual(imgTarget.get(),*__visual_it,false);
                     ++__visual_it;
                 };
        #if !defined(TARGET_WIN32)
                imgTarget->commit();
        #endif
         #if defined(TARGET_WIN32)
                auto effect_it = layer->style->effects.begin();
                while(effect_it !=layer->style->effects.end()){
                    imgTarget->applyEffect(effect_it->get());
                    ++effect_it;
                };
                imgTarget->commitEffects();  
                imgTarget->commit();
                 auto _visual = visualTree->makeVisual(imgTarget);
         #else
                 Core::SharedPtr<BDCompositionImage> img = imgTarget->getImg();
                 auto effect_it = layer->style->effects.begin();
                 while(effect_it !=layer->style->effects.end()){
                     img->applyEffect(effect_it->get());
                     ++effect_it;
                 };
                 img->commitEffects();
                 auto _visual = visualTree->makeVisual(img);
         #endif
                 std::cout << "Visual has been Created... Mounting child" << std::endl;
                 visualTree->addVisual(_visual);
                 context->assignRenderTargetToLayer(layer,imgTarget);
                 ++it;
             };
         }

        // MessageBoxA(HWND_DESKTOP,"Will Render Visual Tree to HWND","NOTE",MB_OK);
        std::cout << "Will Render Visual Tree" << std::endl;
        context->renderVisualTreeToView(visualTree,currentLimb->renderTarget,false);
        visualTrees.insert(std::make_pair(currentLimb->renderTarget,visualTree));
        deviceContexts.insert(std::make_pair(currentLimb->renderTarget,context));
    };
    void BackendImpl::doUpdate(){
//         // MessageBoxA(HWND_DESKTOP,"Do Update","NOTE",MB_OK);
//         auto & tree = visualTrees[currentLimb->renderTarget];
//         auto & rootTarget = layerTargets[currentLimb->limbRoot];
//         auto & rootLayer = currentLimb->limbRoot;
//     #if defined(TARGET_WIN32)
//         if(rootTarget->needsSwapChain()){
//             rootTarget->redoSwapChain();
//             auto r_visual = tree->makeVisual(rootTarget);
//             tree->replaceRootVisual(r_visual);
//         }
//         else if(rootTarget->needsDeviceContext()){
//             rootTarget->redoDeviceContext();
//
//         };
//     #endif
//         /// Redraw Visuals!
//         rootTarget->clear(rootLayer->style->background);
//         auto __visual_it = rootLayer->style->visuals.begin();
//         while(__visual_it != rootLayer->style->visuals.end()){
//             drawVisual(rootTarget.get(),__visual_it->get(),true);
//             ++__visual_it;
//         };
//         rootTarget->commit();
//        
//     #if !defined(TARGET_WIN32)
//         Core::SharedPtr<BDCompositionImage> img = rootTarget->getImg();
//         auto effect_it = rootLayer->style->effects.begin();
//         while(effect_it !=rootLayer->style->effects.end()){
//             img->applyEffect(effect_it->get());
//             ++effect_it;
//         };
//         img->commitEffects();
//     #else
//       auto effect_it = rootLayer->style->effects.begin();
//         while(effect_it !=rootLayer->style->effects.end()){
//             rootTarget->applyEffect(effect_it->get());
//             ++effect_it;
//         };
//         rootTarget->commitEffects();
//     #endif
//
//         auto v_it = currentLimb->begin();
//         while(v_it != currentLimb->end()){
//             auto child = v_it->get();
//             auto & imgTarget = layerTargets[child];
//     #if defined(TARGET_WIN32)
//         if(imgTarget->needsSwapChain()){
//             imgTarget->redoSwapChain();
//             auto _visual = tree->makeVisual(imgTarget);
//             tree->addVisual(_visual);
//             tree->replaceVisualWithTargetPtr(imgTarget,_visual);
//         }
//         else if(imgTarget->needsDeviceContext()){
//             imgTarget->redoDeviceContext();
//
//         };
//     #endif
//         /// Redraw Visuals!
//             imgTarget->clear(child->style->background);
//             auto __visual_it = child->style->visuals.begin();
//             while(__visual_it != child->style->visuals.end()){
//                 drawVisual(imgTarget.get(),__visual_it->get(),true);
//                 ++__visual_it;
//             };
//             rootTarget->commit();
//             Core::SharedPtr<BDCompositionImage> img = imgTarget->getImg();
//             auto effect_it = child->style->effects.begin();
//             while(effect_it != child->style->effects.end()){
//                 img->applyEffect(effect_it->get());
//                 ++effect_it;
//             };
//             img->commitEffects();
//             ++v_it;
//         };
//
//         global_device->renderVisualTreeToView(tree,currentLimb->renderTarget,true);
    };

    void BackendImpl::redoLayout(){
        auto & tree = visualTrees[currentLimb->renderTarget];
        tree->layout();
    };

};
