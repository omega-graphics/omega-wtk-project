#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {

class BDCompositionRenderTarget;
class BDCompositionLayerRenderTarget;
class BDCompositionImage;
class BDCompositionImageRenderTarget;
class BDCompositionFontFactory;

/**
 A (Platform Specific) Composition Device!
 */
class BDCompositionDevice {
protected:
    Core::Map<Layer *,Core::SharedPtr<BDCompositionRenderTarget>> targets;
public:
    // BDCompositionDevice(){};
    static Core::SharedPtr<BDCompositionDevice> Create();
    /**
     Creates a Render Target for a Composition::Layer
     */
    virtual Core::SharedPtr<BDCompositionLayerRenderTarget> makeLayerRenderTarget(Layer *layer) = 0;
    /**
     Creates a Render Target for a `new` BDCompositionImage
     */
    virtual Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect & size) = 0;
    /**
     Creates a Render Target for an existing BDCompositionImage.
     */
    virtual Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> & img) = 0;
    /**
     Creates a Font Factory
     */
    virtual Core::SharedPtr<BDCompositionFontFactory> createFontFactory() = 0;
    virtual void destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target) = 0;
    // virtual ~BDCompositionDevice();
};

};

#endif
