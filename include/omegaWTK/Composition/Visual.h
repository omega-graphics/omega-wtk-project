#include "omegaWTK/Core/Core.h"
#include "Color.h"
#include "Text.h"
#include "Brush.h"
#include "omegaWTK/Core/ImgCodec.h"

#include <algorithm>

#ifndef OMEGAWTK_COMPOSITION_VISUAL_H
#define OMEGAWTK_COMPOSITION_VISUAL_H

namespace OmegaWTK {
    namespace Composition {
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
        Core::SharedPtr<Brush> brush;
        unsigned width;
        Border() = delete;
        Border(Core::SharedPtr<Brush> & _brush,unsigned _width):brush(_brush),width(_width){};
    };
    
    struct LayerEffect {
        typedef enum : OPT_PARAM {
            DropShadow,
            Transformation,
            MotionBlur
        } Type;
        Type type;
        void * params;
        typedef struct {
            float x_offset, y_offset;
            float radius;
            float blurAmount;
        } DropShadowParams;
        typedef struct {
            
        } TransformationParams;
        typedef struct {
            
        } MotionBlurParams;
    };
    
    /// An object drawn by a Compositor.
    struct Visual {
        unsigned id;
        typedef enum : OPT_PARAM {
            Rect,
            RoundedRect,
            Ellipse,
            Text,
            Bitmap
        } Type;
        Type type;
        typedef struct {
            Core::Rect rect;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } RectParams;
        
        typedef struct {
            Core::Rect rect;
            unsigned rad_x;
            unsigned rad_y;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } RoundedRectParams;
        
        typedef struct {
            Core::Ellipse ellipse;
            Core::SharedPtr<Brush> brush;
            Core::Optional<Border> border;
        } EllipseParams;
        typedef struct {
            class Text text;
            Core::SharedPtr<Brush> brush;
            Core::Rect rect;
        } TextParams;
        typedef struct {
            Core::SharedPtr<Core::BitmapImage> img;
            Core::Rect rect;
        } BitmapParams;
        void * params;
        Visual() = delete;
        Visual(unsigned id,Type type,void * params):id(id),type(type),params(params){};
        void setColor(const Color & new_color);
        void setRect(const Core::Rect & bew_rect);
        void setFont(const Text::Font & new_font);
        VPVR getColor();
        VPVR getRect();
        VPVR getFont();
    };
    
#define VISUAL_RECT(rect,brush) Visual::RectParams({rect,brush,{}})
#define VISUAL_RECT_W_FRAME(rect,brush,border) Visual::RectParams({rect,brush,border})

#define VISUAL_IMG(img,rect) Visual::BitmapParams({img,rect})
    class BackendImpl;
    
    class LayerStyle {
        Core::Vector<Core::UniquePtr<Visual>> visuals;
        Core::Vector<SharedHandle<LayerEffect>> effects;
        Color background = Color(Color::White);
        template<class _Ty>
        void _construct_visual(Visual::Type type,_Ty & params){
            visuals.push_back(std::make_unique<Visual>(visuals.size(),type,(void *)new _Ty(params)));
        };
        friend class BackendImpl;
    public:
        LayerStyle();
        /**
         Adds A Rect to the Style!
         */
        void add(Visual::RectParams params);
        /**
         Adds A Rounded Rect to the Style!
         */
        void add(Visual::RoundedRectParams params);
        /**
         Adds An Ellipse to the Style!
         */
        void add(Visual::EllipseParams params);
        /**
         Adds A Bitmap to the Style!
         */
        void add(Visual::BitmapParams params);
        /**
         Adds A Text Object to the Style!
         */
        void add(Visual::TextParams params);
        /**
        Changes/Sets the brush for the visual at the provided index.
        */
        void setBrush(unsigned id,const Core::SharedPtr<Brush> & new_brush);
        void setBackgroundColor(const Color & color){ background = color;};
        void addEffect(SharedHandle<LayerEffect> & effect);
    };
    
    class WindowStyle {
        Core::Vector<Core::UniquePtr<Visual>> visuals;
    public:
        
    };
    
    class MenuStyle {
        
    };
    
};
    
};

#endif
