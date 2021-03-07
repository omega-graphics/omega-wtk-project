#include "omegaWTK/Core/Core.h"
#include "../BDCompositionVisualTree.h"
#include "DXBDCompositionDevice.h"


#ifndef OMEGAWTK_COMPOSITION_DX_DCVISUALTREE_H
#define OMEGAWTK_COMPOSITION_DX_DCVISUALTREE_H

namespace OmegaWTK::Composition {
/**
     DirectX Backend Impl of the BDCompositionVisualTree using IDCompositionVisuals
*/
class DCVisualTree : public BDCompositionVisualTree {
    DXBDCompositionDevice *device;
    Core::UniqueComPtr<IDCompositionTarget> hwndTarget;
    typedef BDCompositionVisualTree Parent;
    friend class DXBDCompositionDevice;
    public:
    struct Visual : Parent::Visual {
        IDCompositionVisual2 * visual;
        Core::SharedPtr<BDCompositionImageRenderTarget> img;
        Core::Position pos;
        ~Visual();
    };
    DCVisualTree(DXBDCompositionDevice *device);
    static Core::SharedPtr<BDCompositionVisualTree> Create(DXBDCompositionDevice *device);
    Core::SharedPtr<Parent::Visual> makeVisual(Core::SharedPtr<BDCompositionImageRenderTarget> & imgRenderTarget);
    void replaceRootVisual(Core::SharedPtr<Parent::Visual> &visual);
    void replaceVisualWithTargetPtr(Core::SharedPtr<BDCompositionImageRenderTarget> & imgTarget,Core::SharedPtr<Parent::Visual> & visual);
    void setRootVisual(Core::SharedPtr<Parent::Visual> &visual);
    void addVisual(Core::SharedPtr<Parent::Visual> &visual);
};

};

#endif