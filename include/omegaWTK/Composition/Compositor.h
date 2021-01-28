#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Composition/Canvas.h"

#ifndef OMEGAWTK_COMPOSITION_COMPOSITOR_H
#define OMEGAWTK_COMPOSITION_COMPOSITOR_H

namespace OmegaWTK {
    namespace Composition {

        class Text {
            Core::String text_val;
            public:
            struct Font {
                typedef enum : OPT_PARAM {
                    Regular,
                    Italic,
                    Bold,
                    BoldAndItalic
                } FontStyle;
                Core::String family;
                OPT_PARAM style;
                Font(Core::String _family,OPT_PARAM _style):family(_family),style(_style){};
                ~Font(){};

            };
            private:
            Font font;
            unsigned fontSize;
            public:
            const Font & getFont() noexcept{
                return font;
            };
            const unsigned getFontSize() noexcept{
                return fontSize;
            };
            void setFontSize(const unsigned & new_size){
                fontSize = new_size;
            };
            void setFont(const Font & new_font){
                font = new_font;
            };
            
            Text(Core::String _val,unsigned size,const Font & _font = Font("Arial",Font::Regular)):text_val(_val),font(std::move(_font)),fontSize(size){};
        };
        class Compositor {
            public:
            Compositor(){};
            virtual void setCanvas(Canvas *canvas);
            virtual void drawRect(Core::Rect & rect,OPT_PARAM type);
            virtual void drawText(Text & text);
            // virtual void DoAnimation(Animation & anim);
            virtual ~Compositor();
        };

        typedef Compositor * CompPtr;
        typedef Canvas * CanvasPtr;

        CompPtr make_compositor();
        CanvasPtr make_canvas(Native::NativeItemPtr native_item);

    };
}

#endif
