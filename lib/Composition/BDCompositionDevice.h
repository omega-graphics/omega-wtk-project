#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {

class BDCompositionRenderTarget;
/**
 A (Platform Specific) Composition Device!
 */
class BDCompositionDevice {
    Core::Map<Layer *,Core::SharedPtr<BDCompositionRenderTarget>> targets;
public:
    static Core::SharedPtr<BDCompositionDevice> Create();
    Core::SharedPtr<BDCompositionRenderTarget> makeTarget(Layer *layer);
    void destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target);
};

};

#endif
