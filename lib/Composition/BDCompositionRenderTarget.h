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
#ifdef TARGET_WIN32
    virtual bool targetIsFresh() = 0;
    virtual bool needsDeviceContext() = 0;
#endif
    Core::SharedPtr<BDCompositionImage> & getImage(unsigned visual_id);
    /// @name Composing Methods
    /// @{
    /**
     Clears the Target's Surface
     */
    virtual void clear(Color &clearColor) = 0;
    /**
     Draws the frame of a rectangle using the provided brush.
     */
    virtual void frameRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush,unsigned width) = 0;
    /**
     Draws a rectangle using the provided brush.
     */
    virtual void fillRect(Core::Rect & rect,Core::SharedPtr<Brush> & brush) = 0;
    /**
     Draws the frame of a rounded rectangle using the provided brush.
     */
    virtual void frameRoundedRect(Core::RoundedRect & rect,Core::SharedPtr<Brush> & brush,unsigned width) = 0;
    /**
     Draws a rounded rectangle using the provided brush.
     */
    virtual void fillRoundedRect(Core::RoundedRect & rect,Core::SharedPtr<Brush> & brush) = 0;
    virtual Core::SharedPtr<BDCompositionImage> createImageFromBitmapImage(Core::SharedPtr<Media::BitmapImage> & img,Core::Rect &newSize,unsigned v_id) = 0;
    virtual void drawImage(Core::SharedPtr<BDCompositionImage> & img,Core::Position pos) = 0;
    virtual void drawText(Core::SharedPtr<BDCompositionFont> & font,Core::String & string,Core::Rect & textRect,Core::SharedPtr<Brush> & brush) = 0;
    virtual void commit() = 0;
    /// @}
    // virtual ~BDCompositionRenderTarget();
};
/**
 Composition interface for rendering to a view.
 */
class BDCompositionViewRenderTarget : public BDCompositionRenderTarget {
public:
#ifdef TARGET_WIN32
    virtual void redoSwapChain() = 0;
    virtual bool needsSwapChain() = 0;
    virtual void redoDeviceContext() = 0;
#endif
};
/**
 Composition interface for rendering to an image
 */
class BDCompositionImageRenderTarget : public BDCompositionRenderTarget {
    
public:
#ifdef TARGET_WIN32
    virtual bool needsSwapChain() = 0;
    virtual void redoDeviceContext() = 0;
    virtual void redoSwapChain() = 0;
    virtual void applyEffect(LayerEffect *effect) = 0;
    virtual void commitEffects() = 0;
#endif
    /**
     Get the underlying image
     @returns A shared handle to a Composition Image
     */
    #if !defined(TARGET_WIN32)
    virtual Core::SharedPtr<BDCompositionImage> getImg() = 0;
    #endif
};

}

#endif
