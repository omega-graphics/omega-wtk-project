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
    
    struct VisualEffect {
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
        Core::Vector<VisualEffect> effects;
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
        Visual(unsigned id,Type type,std::initializer_list<VisualEffect> & effects,void * params):id(id),type(type),effects(effects),params(params){};
        void setColor(const Color & new_color);
        void setRect(const Core::Rect & bew_rect);
        void setFont(const Text::Font & new_font);
        VPVR getColor();
        VPVR getRect();
        VPVR getFont();
    };
    
#define VISUAL_RECT(rect,brush) Visual::RectParams({rect,brush,{}})
#define VISUAL_RECT_W_FRAME(rect,brush,border) Visual::RectParams({rect,brush,border})
    
    class BackendImpl;
    
    class Style {
        Core::Vector<Core::UniquePtr<Visual>> visuals;
        template<class _Ty>
        void _construct_visual(Visual::Type type,_Ty & params,std::initializer_list<VisualEffect> & effects){
            visuals.push_back(std::make_unique<Visual>(visuals.size(),type,effects,(void *)new _Ty(params)));
        };
        friend class BackendImpl;
    public:
        Style();
        /**
         Adds A Rect to the Style!
         */
        void add(Visual::RectParams params,std::initializer_list<VisualEffect> initialEffects);
        /**
         Adds A Rounded Rect to the Style!
         */
        void add(Visual::RoundedRectParams params,std::initializer_list<VisualEffect> initialEffects);
        /**
         Adds An Ellipse to the Style!
         */
        void add(Visual::EllipseParams params,std::initializer_list<VisualEffect> initialEffects);
        /**
         Adds A Bitmap to the Style!
         */
        void add(Visual::BitmapParams params,std::initializer_list<VisualEffect> initialEffects);
        /**
         Adds A Text Object to the Style!
         */
        void add(Visual::TextParams params,std::initializer_list<VisualEffect> initialEffects);
        void setBrush(unsigned id,const Core::SharedPtr<Brush> & new_brush);
        VisualEffect & getVisualEffect(unsigned id,unsigned idx);
    };
    
};
    
};

#endif
