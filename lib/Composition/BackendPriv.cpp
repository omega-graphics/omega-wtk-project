#include "BackendPriv.h"
#include "BDCompositionRenderTarget.h"

namespace OmegaWTK::Composition {
    Core::SharedPtr<BDCompositionDevice> global_device;
    bool hasGlobalDevice = false;

    BackendImpl::BackendImpl(){
        // MessageBoxA(GetForegroundWindow(),"Setting up Backend Private!","NOTE",MB_OK);
        if(!hasGlobalDevice){
            global_device = BDCompositionDevice::Create();
            hasGlobalDevice = true;
        }
    };
    void BackendImpl::drawVisual(Core::SharedPtr<BDCompositionRenderTarget> & target,Visual *visual){
        // MessageBoxA(GetForegroundWindow(),"Will Draw Rect","NOTE",MB_OK);
        switch (visual->type) {
        case Visual::Rect : {
            Visual::RectParams *params = (Visual::RectParams *)visual->params;
            // if(!target->hasBrush(params->brush)){
            //     target->addBrush(params->brush);
            // };
            target->fillRect(params->rect,params->brush);
            if(params->border.has_value()){
                target->frameRect(params->rect,params->border->brush,params->border->width);
            };
            break;
        };
        default: 
            break;
        }
    };
    void BackendImpl::doWork(){
        auto target = global_device->makeTarget(currentLayer);
        auto & visuals = currentLayer->getTargetVisuals();

        for(auto & visual : visuals){
            drawVisual(target,visual);
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