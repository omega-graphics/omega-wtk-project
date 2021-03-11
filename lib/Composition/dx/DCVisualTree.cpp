#include "DCVisualTree.h"
#include <iostream>
#include "DXBDCompositionRenderTarget.h"

namespace OmegaWTK::Composition {
    
    DCVisualTree::DCVisualTree(DXBDCompositionDevice *device):device(device),hwndTarget(nullptr){};

    DCVisualTree::Visual::Visual(IDCompositionVisual2 *v,Core::SharedPtr<BDCompositionImageRenderTarget> &img,Core::Position &pos):visual(v),img(img),pos(pos){
        
    };

    DCVisualTree::Visual::~Visual(){
        visual->RemoveAllVisuals();
        Core::SafeRelease(&visual);
    };

    Core::SharedPtr<BDCompositionVisualTree> DCVisualTree::Create(DXBDCompositionDevice *device){
        return std::make_shared<DCVisualTree>(device);
    };

    Core::SharedPtr<BDCompositionVisualTree::Visual> DCVisualTree::makeVisual(Core::SharedPtr<BDCompositionImageRenderTarget> &img){
        DXBDCompositionImageRenderTarget *dxImgTarget = (DXBDCompositionImageRenderTarget *)img.get();

        
        HRESULT hr;
        IDCompositionVisual2 *v;
        hr = device->dcomp_device_1->CreateVisual(&v);
        if(FAILED(hr)){

        };

        hr = v->SetContent(dxImgTarget->dxgi_swap_chain_3.get());
        if(FAILED(hr)){
            std::stringstream ss;
            ss << std::hex << hr;
            MessageBoxA(HWND_DESKTOP,(std::string("Failed to set Content of Visual. ERROR:") + ss.str()).c_str(),NULL,MB_OK);
        };
        // rc.visual = nullptr;
        return std::make_shared<Visual>(v,img,dxImgTarget->rect.pos);
    };

    void DCVisualTree::setRootVisual(Core::SharedPtr<Parent::Visual> visual){
        root_v = visual;
    };

    void DCVisualTree::replaceRootVisual(Core::SharedPtr<Parent::Visual> visual){
        root_v.reset();
        root_v = visual;
    };

    void DCVisualTree::replaceVisualWithTargetPtr(Core::SharedPtr<BDCompositionImageRenderTarget> &imgTarget,Core::SharedPtr<Parent::Visual>  visual){
        for(auto & v : body){
            Visual *_v = (Visual *)v.get();
            if(_v->img.get() == imgTarget.get()){
                v.reset();
                v = visual;
                break;
            };
        };
    };

    void DCVisualTree::addVisual(Core::SharedPtr<Parent::Visual> visual){
        body.push_back(visual);
    };

    void DCVisualTree::layout(){

    };

};