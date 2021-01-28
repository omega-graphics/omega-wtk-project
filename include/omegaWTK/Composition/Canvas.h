#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_CANVAS_H
#define OMEGAWTK_COMPOSITION_CANVAS_H

namespace OmegaWTK::Composition {
    class Layer;

    struct CanvasDrawOpts {

    };

    /// A specified area (upon which there are several layers) on the screen bound to a native item that can be drawn on by a parent Compositor.
    /// 
    class Canvas {
        unsigned w , h;
        Core::QueueVector<Layer *> layers;
        Layer * current_layer;
    public:
        void addLayer(Layer * layer);
        void insertLayer(Layer * layer,unsigned idx);
        void setCurrentLayer(unsigned idx);
        Canvas() = delete;
        Canvas(Canvas &&) = delete;
        Canvas(unsigned width,unsigned height,const CanvasDrawOpts & opts = CanvasDrawOpts());

        // virtual Native::NativeItemPtr getNativeItem();
    };
};



#endif