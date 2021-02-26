#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Composition/Brush.h"
#include "omegaWTK/Composition/Layer.h"
#include "BDCompositionImage.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONRENDERTARGET_H

namespace OmegaWTK::Composition {

class BDCompositionDevice;
class BDCompositionFont;
/**
 A Composition Render Target.
 */
class BDCompositionRenderTarget {
protected:
    Core::Map<unsigned,Core::SharedPtr<BDCompositionImage>> images;
public:
    //static Core::SharedPtr<BDCompositionRenderTarget> Create(BDCompositionDevice * device,Layer *layer);
#ifdef TARGET_WIN32
    virtual bool targetIsFresh() = 0;
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
    Core::SharedPtr<BDCompositionImage> & getImage(unsigned visual_id);
    /// @name Composing Methods
    /// @{
    virtual void clear(Color &clearColor) = 0;
    virtual void frameRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush,unsigned width) = 0;
    virtual void fillRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush) = 0;
    virtual void frameRoundedRect(Core::RoundedRect & rect,Core::SharedPtr<Brush> & brush,unsigned width) = 0;
    virtual void fillRoundedRect(Core::RoundedRect & rect,Core::SharedPtr<Brush> & brush) = 0;
    virtual Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Core::BitmapImage> & img,Core::Rect &newSize,unsigned v_id) = 0;
    virtual void drawImage(Core::SharedPtr<BDCompositionImage> & img,Core::Position pos) = 0;
    virtual void drawText(Core::SharedPtr<BDCompositionFont> & font,Core::String & string,Core::Rect & textRect,Core::SharedPtr<Brush> & brush) = 0;
    virtual void commit() = 0;
    /// @}
    // virtual ~BDCompositionRenderTarget();
};

}

#endif