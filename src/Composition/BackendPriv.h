#include "BDCompositionDevice.h"
#include "omegaWTK/Composition/Backend.h"

#ifndef OMEGAWTK_COMPOSITION_BACKENDPRIV_H
#define OMEGAWTK_COMPOSITION_BACKENDPRIV_H

namespace OmegaWTK::Composition {

class BackendImpl : public Backend {
    Core::Map<ViewRenderTarget *,Core::SharedPtr<BDCompositionVisualTree>> visualTrees;
    Core::Map<ViewRenderTarget *,Core::SharedPtr<BDCompositionDeviceContext>> deviceContexts;
    void drawVisual(BDCompositionRenderTarget * target,Visual * visual,bool updatePass = true);
    public:
    void doWork();
    void doUpdate();
    void redoLayout();
    BackendImpl();
};

};
#endif
