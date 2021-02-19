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
    Core::Vector<Core::SharedPtr<Brush>> brushes;
public:
    static Core::SharedPtr<BDCompositionRenderTarget> Create(BDCompositionDevice * device,Layer *layer);
    /**
     Adds a Brush to the Render Target's Brush Catalog.
     */
    void addBrush(Core::SharedPtr<Brush> & brush);
    /**
     Removes a Brush from the Render Target's Brush Catalog.
     */
    void removeBrush(Core::SharedPtr<Brush> & brush);
    /// @name Composing Methods
    /// @{
    virtual void clear(Color &clearColor);
    virtual void frameRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush,unsigned width);
    virtual void fillRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush);
    virtual void commit();
    /// @}
};

}

#endif
