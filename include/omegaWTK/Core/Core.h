#include <string>
#include <vector>
#include <map>

#ifndef OMEGAWTK_CORE_CORE_H
#define OMEGAWTK_CORE_CORE_H

namespace OmegaWTK {
    namespace Core {
        typedef unsigned char Option;
    };
    #define STATIC_OPT static constexpr Core::Option
    #define OPT_PARAM Core::Option
    #define ENUM(name,args...) enum class name : Core::Option { args }; 
    #define CORE_CLASS(name) class OWTK##name

    namespace Core {
    
        typedef std::string String;
    
        template<class _Ty>
        using Vector = std::vector<_Ty>;

        template<class _Key,class _Val>
        using Map = std::map<_Key,_Val>;

        struct Position {
            unsigned x;
            unsigned y;
        };
        struct Dimensions {
            unsigned minWidth;
            unsigned minHeight;
        };
        struct Rect {
            enum : OPT_PARAM {
                Regular,
                Rounded
            };
            Position pos;
            Dimensions dimen;
            Rect(Position _pos,Dimensions _dimen):pos(_pos),dimen(_dimen){};
        };
        class Text {
            String text_val;
            public:
            struct Font {
                enum : OPT_PARAM {
                    Regular,
                    Italic,
                    Bold,
                    BoldAndItalic
                };
                String family;
                OPT_PARAM style;
                Font(String _family,OPT_PARAM _style):family(_family),style(_style){};
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
            
            Text(String _val,unsigned size,const Font & _font = Font("Arial",Font::Regular)):text_val(_val),font(std::move(_font)),fontSize(size){};
        };
        /// A basic reimplementation of the std::string class!
        // CORE_CLASS(String) {
        //     char * data;
        //     unsigned len;
        //     bool compare(OWTKString & other);
        //     void _realloc_data(size_t new_size);
        //     public:
        //     using iterator = char *;
        //     using const_iterator = const char *;
        //     using reference = char &;
        //     iterator begin(){
        //         return iterator(data);
        //     };
        //     iterator end(){
        //         return iterator(data) + len;
        //     };
        //     reference operator[](unsigned idx){
        //         return begin()[idx];
        //     };
        //     bool operator==(OWTKString & str){
        //         return compare(str);
        //     };
        //     bool operator!=(OWTKString & str){
        //         return !compare(str);
        //     };
        //     OWTKString & append(const char * __c_str);
        //     OWTKString & append(OWTKString & str);
        //     const unsigned & size(){
        //       return len;  
        //     };
        //     OWTKString() = default;
        //     OWTKString(const char * c_str);
        //     OWTKString(char * data,unsigned _len);
        //     ~OWTKString();
        // };
    };
};

#endif
