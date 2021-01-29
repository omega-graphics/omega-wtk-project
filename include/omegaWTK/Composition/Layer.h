#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_COMPOSITION_LAYER_H
#define OMEGAWTK_COMPOSITION_LAYER_H

namespace OmegaWTK {
    namespace Composition {

        /// A surface upon which visuals can be drawn. (A Canvas!!)
        /// `INTERFACE`
        class Layer {
            Native::NativeItemPtr native_binding;
            std::vector<Layer *> children;
            Layer * parent_ptr = nullptr;
            Core::Rect surface_rect;
            bool enabled;
            public:
            const Core::Rect & getLayerRect(){return surface_rect;};
            void setEnabled(bool state){enabled = state;};
            bool isChildLayer(){return parent_ptr != nullptr;};
            void addSubLayer(Layer *layer);
            void removeSubLayer(Layer *layer);
            Layer(const Core::Rect & rect,Native::NativeItemPtr native_ptr);
        };
    };
};

#endif