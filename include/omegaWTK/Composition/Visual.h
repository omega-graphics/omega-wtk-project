#include "omegaWTK/Core/Core.h"
#include "Color.h"
#include "Text.h"
#include "Brush.h"
#include "omegaWTK/Core/ImgCodec.h"

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
        typedef struct {
            Core::SharedPtr<Core::BitmapImage> img;
            Core::Rect rect;
        } BitmapParams;
        void * params;
        void setColor(const Color & new_color);
        void setRect(const Core::Rect & bew_rect);
        void setFont(const Text::Font & new_font);
        VPVR getColor();
        VPVR getRect();
        VPVR getFont();
    };
    };
};

#endif
