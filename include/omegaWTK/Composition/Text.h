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
                typedef enum : OPT_PARAM {
                    LeftUpper,
                    LeftCenter,
                    LeftLower,
                    MiddleUpper,
                    MiddleCenter,
                    MiddleLower,
                    RightUpper,
                    RightCenter,
                    RightLower
                }Alignment;
                typedef enum : OPT_PARAM {
                    None,
                    WrapByWord,
                    WrapByCharacter
                } Wrapping;
                Core::String family;
                FontStyle style;
                Alignment textAlignment;
                Wrapping wrapping;
                Font(Core::String _family,FontStyle _style,Alignment textAlignment = LeftUpper,Wrapping wrapping = WrapByWord):family(_family),style(_style),textAlignment(textAlignment),wrapping(wrapping){};
                ~Font(){};

            };
            private:
            Font font;
            unsigned fontSize;
            public:
            Font & getFont() noexcept{
                return font;
            };
            unsigned getFontSize() noexcept{
                return fontSize;
            };
            void setFontSize(const unsigned & new_size){
                fontSize = new_size;
            };
            void setFont(const Font & new_font){
                font = new_font;
            };
            Core::String & getString() noexcept{ return text_val;};
            void setString(const Core::String & str){
                text_val = std::move(str);
            };
            
            Text(Core::String _val,unsigned size,const Font & _font):text_val(_val),font(_font),fontSize(size){};
        };
    };
}

#endif
