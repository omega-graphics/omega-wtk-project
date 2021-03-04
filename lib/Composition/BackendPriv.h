#include "BDCompositionDevice.h"
#include "omegaWTK/Composition/Backend.h"

#ifndef OMEGAWTK_COMPOSITION_BACKENDPRIV_H
#define OMEGAWTK_COMPOSITION_BACKENDPRIV_H

namespace OmegaWTK::Composition {

class BackendImpl : public Backend {
    Core::Map<Layer *,Core::SharedPtr<BDCompositionImageRenderTarget>> layerTargets;
    Core::Map<ViewRenderTarget *,Core::SharedPtr<BDCompositionVisualTree>> visualTrees;
    void drawVisual(BDCompositionRenderTarget * target,Visual * visual,bool updatePass = true);
    public:
    void doWork();
    void doUpdate();
    BackendImpl();
};

};
#endif
