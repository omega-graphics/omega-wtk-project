#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONDEVICE_H

namespace OmegaWTK::Composition {

class BDCompositionRenderTarget;
class BDCompositionViewRenderTarget;
class BDCompositionImage;
class BDCompositionImageRenderTarget;
//class BDCompositionFontFactory;
class BDCompositionVisualTree;

class BDCompositionDeviceContext;

/**
 A (Platform Specific) Composition Device!
 */
class BDCompositionDevice {
public:
    // BDCompositionDevice(){};
    static Core::SharedPtr<BDCompositionDevice> Create();
    /**
     Creates a Font Factory
     */
//    virtual Core::SharedPtr<BDCompositionFontFactory> createFontFactory() = 0;
    virtual Core::SharedPtr<BDCompositionDeviceContext> createContext() = 0;
};

class BDCompositionDeviceContext {
protected:
    Core::Map<Layer *,Core::SharedPtr<BDCompositionRenderTarget>> targets;
public:
    /**
     Creates a Render Target for a `new` BDCompositionImage
     */
    virtual Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::Rect & size) = 0;
    /**
     Creates a Render Target for an existing BDCompositionImage.
     */
    virtual Core::SharedPtr<BDCompositionImageRenderTarget> makeImageRenderTarget(Core::SharedPtr<BDCompositionImage> & img) = 0;
    /**
     Assigns an Image Render Target to a Layer.
     NOTE: This method must be invoked before invoking @see layerForRenderTarget with the same layer.
     */
    void assignRenderTargetToLayer(Layer *layer,Core::SharedPtr<BDCompositionImageRenderTarget> & target);
    /**
     Gets an Image Render Target associated with a Layer
     */
    BDCompositionImageRenderTarget *layerForRenderTarget(Layer *layer);
    /**
     Creates a Visual Tree
     */
    virtual Core::SharedPtr<BDCompositionVisualTree> createVisualTree() = 0;
    virtual void renderVisualTreeToView(Core::SharedPtr<BDCompositionVisualTree> & visualTree,ViewRenderTarget *view,bool updatePass) = 0;
    virtual void destroyTarget(Layer *layer,Core::SharedPtr<BDCompositionRenderTarget> &target) = 0;
    // virtual ~BDCompositionDevice();
};

};

#endif
