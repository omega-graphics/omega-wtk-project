#include "BDCompositionDevice.h"
#include "omegaWTK/Composition/Backend.h"

#ifndef OMEGAWTK_COMPOSITION_BACKENDPRIV_H
#define OMEGAWTK_COMPOSITION_BACKENDPRIV_H

namespace OmegaWTK::Composition {

class BackendImpl : public Backend {
    Core::Map<Layer *,Core::SharedPtr<BDCompositionRenderTarget>> targets;
    void drawVisual(Core::SharedPtr<BDCompositionRenderTarget> & target,Visual * visual);
    public:
    void doWork();
    void doUpdate();
    BackendImpl();
};

};
#endif