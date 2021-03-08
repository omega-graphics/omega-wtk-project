#include "DCVisualTree.h"
#include <iostream>
#include "DXBDCompositionRenderTarget.h"

namespace OmegaWTK::Composition {
    
    DCVisualTree::DCVisualTree(DXBDCompositionDevice *device):device(device),hwndTarget(nullptr){};

    DCVisualTree::Visual::~Visual(){
        visual->RemoveAllVisuals();
        Core::SafeRelease(&visual);
    };

    Core::SharedPtr<BDCompositionVisualTree> DCVisualTree::Create(DXBDCompositionDevice *device){
        return std::make_shared<DCVisualTree>(device);
    };

    Core::SharedPtr<BDCompositionVisualTree::Visual> DCVisualTree::makeVisual(Core::SharedPtr<BDCompositionImageRenderTarget> &img){
        DXBDCompositionImageRenderTarget *dxImgTarget = (DXBDCompositionImageRenderTarget *)img.get();
        IDCompositionVisual *v;

        HRESULT hr;

        Visual rc;
        hr = device->dcomp_device_1->CreateVisual(&v);
        if(FAILED(hr)){

        };

        hr = v->SetContent(dxImgTarget->dxgi_surface.get());

        rc.visual = v;
        rc.img = img;
        return std::make_shared<Visual>(rc);
    };

    void DCVisualTree::setRootVisual(Core::SharedPtr<Parent::Visual> &visual){
        root_v = visual;
    };

    void DCVisualTree::replaceRootVisual(Core::SharedPtr<Parent::Visual> &visual){
        root_v.reset();
        root_v = visual;
    };

    void DCVisualTree::replaceVisualWithTargetPtr(Core::SharedPtr<BDCompositionImageRenderTarget> &imgTarget,Core::SharedPtr<Parent::Visual> & visual){
        for(auto & v : body){
            Visual *_v = (Visual *)v.get();
            if(_v->img.get() == imgTarget.get()){
                v.reset();
                v = visual;
                break;
            };
        };
    };

    void DCVisualTree::addVisual(Core::SharedPtr<Parent::Visual> &visual){
        HRESULT hr;
        UINT dpi = GetDpiForWindow(GetForegroundWindow());
        FLOAT scaleFactor = FLOAT(dpi)/96.f;
        Visual *root = (Visual *)root_v.get();
        Visual *child = (Visual *)visual.get();
        hr = root->visual->AddVisual(child->visual,TRUE,NULL);
        if(FAILED(hr)){

        };
        hr = child->visual->SetOffsetX(child->pos.x * scaleFactor);
        if(FAILED(hr)){

        };
        hr = child->visual->SetOffsetY(child->pos.y * scaleFactor);
        if(FAILED(hr)){

        };
    };

};