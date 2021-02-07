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
            const Core::String & getString() noexcept{ return text_val;};
            void setString(const Core::String & str){
                text_val = std::move(str);
            };
            
            Text(Core::String _val,unsigned size,const Font & _font):text_val(_val),font(_font),fontSize(size){};
        };
    };
}

#endif
