#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Brush.h"
#include "omegaWTK/Composition/Layer.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONRENDERTARGET_H

namespace OmegaWTK::Composition {

class BDCompositionDevice;
/**
 A Composition Render Target.
 - NOTE: If one draws a geometric primitive with a brush not in its catalog, error will be thrown!
 */
class BDCompositionRenderTarget {
protected:
    // Core::Vector<Core::SharedPtr<Brush>> brushes;
public:
    //static Core::SharedPtr<BDCompositionRenderTarget> Create(BDCompositionDevice * device,Layer *layer);
#ifdef TARGET_WIN32
    virtual bool needsSwapChain() = 0;
    virtual bool needsDeviceContext() = 0;
    virtual void redoSwapChain() = 0;
    virtual void redoDeviceContext() = 0;
#endif
    // /**
    //  Adds a Brush to the Render Target's Brush Catalog.
    //  */
    // void addBrush(Core::SharedPtr<Brush> & brush);
    // /**
    //  Removes a Brush from the Render Target's Brush Catalog.
    //  */
    // void removeBrush(Core::SharedPtr<Brush> & brush);
    // bool hasBrush(Core::SharedPtr<Brush> & brush);
    /// @name Composing Methods
    /// @{
    virtual void clear(Color &clearColor) = 0;
    virtual void frameRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush,unsigned width) = 0;
    virtual void fillRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush) = 0;
    virtual void commit() = 0;
    /// @}
    // virtual ~BDCompositionRenderTarget();
};

}

#endif
