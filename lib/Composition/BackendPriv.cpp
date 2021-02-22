#include "BackendPriv.h"
#include "BDCompositionRenderTarget.h"
#include "BDCompositionFontFactory.h"

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
    void BackendImpl::drawVisual(Core::SharedPtr<BDCompositionRenderTarget> & target,Visual *visual,bool updatePass){
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
        auto target = global_device->makeTarget(currentLayer);
        auto & visuals = currentLayer->getTargetVisuals();
        
        target->clear(currentLayer->getBackgroundColor());
        for(auto & visual : visuals){
            drawVisual(target,visual,false);
        };
        target->commit();
        targets.insert(std::make_pair(currentLayer,target));

    };
    void BackendImpl::doUpdate(){
        auto & target = targets[currentLayer];

        #ifdef TARGET_WIN32
        if(target->needsSwapChain()){
            target->redoSwapChain();
        }
        else if(target->needsDeviceContext()){
            target->redoDeviceContext();
        };
        #endif

        target->clear(currentLayer->getBackgroundColor());
        auto & visuals = currentLayer->getTargetVisuals();
        for(auto & visual : visuals){
            drawVisual(target,visual);
        };
        target->commit();
    };
};
