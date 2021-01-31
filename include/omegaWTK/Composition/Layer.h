/**
 @file Layer.h
 
 Defines the Composition::Layer.
 */

#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Native/NativeCompHandler.h"

#ifndef OMEGAWTK_COMPOSITION_LAYER_H
#define OMEGAWTK_COMPOSITION_LAYER_H

namespace OmegaWTK {
    namespace Composition {
    
    class Color;
        /**
            A surface which visuals can draw upon!
         */
        class Layer : public Native::NativeCompositionHandler {
            Core::Vector<Layer *> children;
            Layer * parent_ptr = nullptr;
            Core::Rect surface_rect;
            bool enabled;
            friend class LayerTree;
            public:
            /// @name Base Functions
            /// @{
            const Core::Rect & getLayerRect(){return surface_rect;};
            void setEnabled(bool state){enabled = state;};
            bool isChildLayer(){return parent_ptr != nullptr;};
            void addSubLayer(Layer *layer);
            void removeSubLayer(Layer *layer);
            /// @}
            
            /// @name Composing Functions!
            /// @{
            void DrawRect(const Core::Rect &rect,const Color & color);
            void DrawRoundedRect(const Core::RoundedRect & rect,const Color & color);
            /// @}
            
            /// @name Main Action Functions!
            /// @{
            void render();
            void cleanup();
            /// @}
            
            Layer(const Core::Rect & rect,Native::NativeItemPtr native_ptr);
            ~Layer();
        };
    
    typedef std::function<bool(Layer *)> LayerTreeTraversalCallback;
        
        class LayerTree {
            Layer *root;
            void _recursive_trav(LayerTreeTraversalCallback &callback,Layer *current);
        public:
            void traverse(LayerTreeTraversalCallback callback);
            bool hasChildLayer(Layer *layer);
            LayerTree(Layer *_root);
            ~LayerTree();
        };
    };
};

#endif
