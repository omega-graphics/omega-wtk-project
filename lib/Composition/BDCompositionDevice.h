#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {

class BDCompositionRenderTarget;
class BDCompositionFontFactory;
/**
 A (Platform Specific) Composition Device!
 */
class BDCompositionDevice {
    Core::Map<Layer *,Core::SharedPtr<BDCompositionRenderTarget>> targets;
public:
    // BDCompositionDevice(){};
    static Core::SharedPtr<BDCompositionDevice> Create();
    virtual Core::SharedPtr<BDCompositionRenderTarget> makeTarget(Layer *layer) = 0;
    virtual Core::SharedPtr<BDCompositionFontFactory> createFontFactory() = 0;
    virtual void destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target) = 0;
    // virtual ~BDCompositionDevice();
};

};

#endif
