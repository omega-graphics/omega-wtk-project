#include "DCVisualTree.h"
#include <iostream>
#include "NativePrivate/win/HWNDItem.h"



namespace OmegaWTK::Composition {

    IDCompositionDevice2 *comp_device = nullptr;
    // IDCompositionDevice3 *comp_device_2 = nullptr;

    Core::SharedPtr<BackendVisualTree> CreateVisualTree(){
        return DCVisualTree::Create();
    };

    OmegaGTE::NativeRenderTargetDescriptor * makeDescForViewRenderTarget(ViewRenderTarget *renderTarget){
        auto desc = new OmegaGTE::NativeRenderTargetDescriptor;
        auto hwndItem =  (Native::Win::HWNDItem *)renderTarget->getNativePtr();
        desc->hwnd = hwndItem->hwnd;
        desc->isHwnd = true;
        return desc;
    };
    OmegaGTE::NativeRenderTargetDescriptor * makeDescForCanvasSurface(CanvasSurface *surface){
       auto desc = new OmegaGTE::NativeRenderTargetDescriptor;
       desc->isHwnd = false;
       auto & rect = surface->getParentLayer()->getLayerRect();

       UINT dpi = GetDpiFromDpiAwarenessContext(GetThreadDpiAwarenessContext());
       float scaleFactor = FLOAT(dpi)/96.f;

       desc->height = rect.h * scaleFactor;
       desc->width = rect.w * scaleFactor;
       return desc;
    };

    
    DCVisualTree::DCVisualTree():hwndTarget(nullptr){
        if(comp_device == nullptr) {
            IDCompositionDesktopDevice *dev;
            auto hr = DCompositionCreateDevice3(NULL,IID_PPV_ARGS(&dev));
            if(FAILED(hr)){

            };
            comp_device = dev;
        }
    };

    // DCVisualTree::Visual::Visual(IDCompositionVisual2 *v,Core::Position &pos):visual(v),img(img),pos(pos){
        
    // };

    DCVisualTree::Visual::~Visual(){
        visual->RemoveAllVisuals();
        Core::SafeRelease(&visual);
    };

    Core::SharedPtr<BackendVisualTree::Visual> DCVisualTree::makeVisual(GERenderTargetContext & renderContext,
                                                            OmegaGTE::NativeRenderTargetDescriptor & targetDesc,
                                                            Core::Position & pos){
        
        auto imgTarget = (OmegaGTE::GENativeRenderTarget *)renderContext.getRenderTarget();
        IDXGISwapChain3 *swapChain = (IDXGISwapChain3 *)imgTarget->getSwapChain();

        
        HRESULT hr;
        IDCompositionVisual2 *v;
        hr = comp_device->CreateVisual(&v);
        if(FAILED(hr)){

        };

        hr = v->SetContent(swapChain);
        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << hr;
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to set Content of Visual. ERROR:") + ss.str()).c_str(),NULL,MB_OK);
        };
        // rc.visual = nullptr;
        auto _v = new Visual{renderContext};
        _v->pos = pos;
        _v->visual = v;
        return std::shared_ptr<Visual>(_v);
    };

    void DCVisualTree::setRootVisual(Core::SharedPtr<Parent::Visual> & visual){
        root = visual;
    };


    void DCVisualTree::addVisual(Core::SharedPtr<Parent::Visual> & visual){
        body.push_back(visual);
    };


     Core::SharedPtr<BackendVisualTree> DCVisualTree::Create(){
        return std::make_shared<DCVisualTree>();
    };

};