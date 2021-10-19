#include "DCVisualTree.h"
#include <iostream>
#include <memory>
#include "NativePrivate/win/HWNDItem.h"



namespace OmegaWTK::Composition {

    IDCompositionDevice2 *comp_device = nullptr;
    IDCompositionDesktopDevice *comp_device_desktop = nullptr;
    // IDCompositionDevice3 *comp_device_2 = nullptr;

    SharedHandle<BackendVisualTree> BackendVisualTree::Create(SharedHandle<ViewRenderTarget> & view){
        return SharedHandle<BackendVisualTree>(new DCVisualTree(view));
    }

    
    DCVisualTree::DCVisualTree(SharedHandle<ViewRenderTarget> & view){
        if(comp_device == nullptr) {
            IDCompositionDesktopDevice *dev;
            auto hr = DCompositionCreateDevice3(NULL,IID_PPV_ARGS(&dev));
            if(FAILED(hr)){

            };
            comp_device = dev;
            comp_device_desktop = dev;
        }

        auto hwndItem = std::dynamic_pointer_cast<Native::Win::HWNDItem>(view->getNativePtr());
        HRESULT res = comp_device_desktop->CreateSurfaceFromHwnd(hwndItem->hwnd,&hwndTarget.comPtr);
    };

    // DCVisualTree::Visual::Visual(IDCompositionVisual2 *v,Core::Position &pos):visual(v),img(img),pos(pos){
        
    // };

    DCVisualTree::Visual::~Visual(){
        visual->RemoveAllVisuals();
        Core::SafeRelease(&visual);
    };

    Core::SharedPtr<BackendVisualTree::Visual> DCVisualTree::makeVisual(Core::Rect & rect,Core::Position & pos){
        
        OmegaGTE::NativeRenderTargetDescriptor desc {};
        desc.isHwnd = false;
        desc.hwnd = NULL;
        desc.height = rect.h;
        desc.width = rect.w;

        auto target = gte.graphicsEngine->makeNativeRenderTarget(desc);
        auto swapChain = (IDXGISwapChain3 *)target->getSwapChain();

        
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
        auto _v = new DCVisualTree::Visual {};
        _v->pos = pos;
        _v->visual = v;
        _v->renderTarget = target;
        return std::shared_ptr<BackendVisualTree::Visual>(_v);
    };

    void DCVisualTree::setRootVisual(Core::SharedPtr<Parent::Visual> & visual){
        root = visual;
    };


    void DCVisualTree::addVisual(Core::SharedPtr<Parent::Visual> & visual){
        body.push_back(visual);
    };


    // void BackendCompRenderTarget::renderVisualTree(){

    // };

};