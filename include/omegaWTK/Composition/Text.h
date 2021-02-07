#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_COMPOSITION_TEXT_H
#define OMEGAWTK_COMPOSITION_TEXT_H

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
                FontStyle style;
                Font(Core::String _family,FontStyle _style):family(_family),style(_style){};
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
    };
}

#endif
