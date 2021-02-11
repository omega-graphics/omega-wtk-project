/**
 @file Layer.h
 
 Defines the Composition::Layer.
 */

#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include "Color.h"
#include "Text.h"
#include <functional>

#ifndef OMEGAWTK_COMPOSITION_LAYER_H
#define OMEGAWTK_COMPOSITION_LAYER_H

namespace OmegaWTK {
    namespace Composition {

    class Compositor;
    /// Visual Params Validation Result!
    struct VPVR {
        typedef enum : int {
            Success = 0,
            Failed = -1
        } Code;
        Code code;
        void * res;
        template<class _Ty>
        _Ty & getRes(){ return *((_Ty *)res);};
            
    };

    struct Border {
        Color color;
        unsigned width;
        Border() = delete;
        Border(Color _color,unsigned _width):color(_color),width(_width){};
    };
    /// An object drawn by a Compositor.
    struct Visual {
        unsigned id;
        typedef enum : OPT_PARAM {
            Rect,
            RoundedRect,
            Ellipse,
            Text
        } Type;
        Type type;
        typedef struct {
            Core::Rect rect;
            Color color;
            Core::Optional<Border> border;
        } RectParams;
        
        typedef struct {
            Core::Rect rect;
            unsigned rad_x;
            unsigned rad_y;
            Color color;
            Core::Optional<Border> border;
        } RoundedRectParams;
        
        typedef struct {
            Core::Ellipse ellipse;
            Color color;
            Core::Optional<Border> border;
        } EllipseParams;
        typedef struct {
            class Text text;
            Color color;
            Core::Rect rect;
        } TextParams;
        void * params;
        void setColor(const Color & new_color);
        void setRect(const Core::Rect & bew_rect);
        void setFont(const Text::Font & new_font);
        VPVR getColor();
        VPVR getRect();
        VPVR getFont();
    };
    
    class Target {
        unsigned id_gen = 0;
        Core::Vector<Visual *> visuals;
        Native::NativeItemPtr native;
        friend class Layer;
    public:
        Target(Native::NativeItemPtr _native);
        ~Target();
    };
        /**
            A surface which visuals can draw upon!
         */
        class OMEGAWTK_EXPORT Layer : public Native::NativeLayer {
            Core::Vector<Layer *> children;
            Layer * parent_ptr = nullptr;
            Core::Rect surface_rect;
            Color background = Color(Color::White);
            Target * compTarget;
            bool enabled;
            Compositor *ownerCompositor;
            friend class LayerTree;
            public:
            /// @name Base Functions
            /// @{
            Native::NativeItemPtr getTargetNativePtr(){return compTarget->native;};
            auto & getTargetVisuals(){return compTarget->visuals;};
            
            auto & getVisualByIdx(unsigned idx){ return compTarget->visuals[idx];};
            const Core::Rect & getLayerRect(){return surface_rect;};
            void setEnabled(bool state){enabled = state;};
            bool isChildLayer(){return parent_ptr != nullptr;};
            void addSubLayer(Layer *layer);
            void removeSubLayer(Layer *layer);
            /// @}
            
            /// @name Composing Functions!
            /// Draws on to its target!
            /// @{
            void setBackgroundColor(const Color & color);

            void drawRect(const Core::Rect &rect,const Color & color,Core::Optional<Border> border = {});

            void drawRoundedRect(const Core::RoundedRect & rect,const Color & color,Core::Optional<Border> border = {});

            void drawText(const Core::String & str,unsigned size,const Color & color,const Core::Rect & rect,const Text::Font & font = {"Arial",Text::Font::Regular});

            void drawEllipse(const Core::Ellipse &ellipse,const Color &color,Core::Optional<Border> border = {});
            /// @}
            
            /// @name Main Action Functions!
            /// @{
            void redraw();
            /// @}
            
            
            Layer(const Core::Rect & rect,Native::NativeItemPtr native_ptr,Compositor *compPtr);
            ~Layer();
        };
    
    typedef std::function<bool(Layer *)> LayerTreeTraversalCallback;
        
        class OMEGAWTK_EXPORT LayerTree {
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
