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
        // DXBDCompositionImageRenderTarget *dxImgTarget = (DXBDCompositionImageRenderTarget *)img.get();

        // HRESULT hr;

        Visual rc;

        rc.pos = {0,0};
        rc.img = img;
        // rc.visual = nullptr;
        return std::make_shared<Visual>(rc);
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

};