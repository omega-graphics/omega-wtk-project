#include "omegaWTK/Core/Core.h"
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>




#ifdef TARGET_WIN32
#include <windows.h>
#endif

#ifdef TARGET_MACOS
#include <unistd.h>
#include "FSCocoa.h"
#endif

namespace OmegaWTK::Core {

bool Rect::compare(Rect & other){
    return (pos.x == other.pos.x) && (pos.y == other.pos.y) && (dimen.minWidth == other.dimen.minWidth) && (dimen.minHeight == other.dimen.minHeight);
};

bool RoundedRect::compare(RoundedRect & other){
    return Rect::compare(other) && (radius_x == other.radius_x) && (radius_y == other.radius_y);
};

bool Ellipse::compare(Ellipse & other){
    return (pos.x == other.pos.x) && (pos.y == other.pos.y) && (radius_x == other.radius_x) && (radius_y == other.radius_y);
};


    // OWTKString::~OWTKString(){
    //     delete data;
    // };

    // bool OWTKString::compare(OWTKString & other){
    //     if(other.size() != size()){
    //         return false;
    //     };

    //     bool returncode = true;

    //     for(unsigned idx = 0;idx < size();++idx){
    //         if(operator[](idx) != other[0]){
    //             returncode = false;
    //             break;
    //         }
    //     };
    //     return returncode;
    // };

    // void OWTKString::_realloc_data(size_t new_size){
    //     char * str_ptr = new char[new_size];
    //     std::move(data,data + len,str_ptr);
    //     delete data;
    //     data = str_ptr;
    // };

    // OWTKString & OWTKString::append(const char * __c_str){
    //     size_t other_str_size = strlen(__c_str);
    //     _realloc_data(len + other_str_size);
    //     std::move(__c_str,__c_str + other_str_size,data + len);
    //     len += other_str_size;
    // };

    // OWTKString & OWTKString::append(OWTKString & str){
    //     _realloc_data(len + str.len);
    //     std::move(str.begin(),str.end(),data + len);
    //     len += str.size();
    // };

    // OWTKString::OWTKString(const char * str){
    //     size_t _size = strlen(str);
    //     data = new char[_size];
    //     std::move(str,str + _size,begin());
    //     len = _size;
        
    // };
    // OWTKString::OWTKString(char * _data,unsigned _len){
    //     size_t _size = _len;
    //     data = new char[_size];
    //     std::move(_data,_data + _size,begin());
    //     len = _len;
    // };
}

namespace OmegaWTK {

Core::Rect Rect(unsigned x,unsigned y,unsigned width,unsigned height){
    return {{x,y},{width,height}};
};
Core::Ellipse Ellipse(unsigned x,unsigned y,unsigned radius_x,unsigned radius_y){
    return {{x,y},radius_x,radius_y};
};

Core::RoundedRect RoundedRect(unsigned x,unsigned y,unsigned width,unsigned height,unsigned radius_x,unsigned radius_y) {
    return {Rect(x,y,width,height),radius_x,radius_y};
}

void FSPath::parse(const Core::String & str){
    unsigned idx = 0;
    
    char buffer[200];
    char *buffer_ptr = buffer;
    char *buf_start = buffer_ptr;
    
    auto get_char = [&](){
        return str[idx];
    };
    
    auto ahead_char = [&](){
        return str[idx + 1];
    };
    
    auto clear_buffer = [&](Token::Type ty){
        auto len = buffer_ptr - buf_start;
        tokens.push_back({ty,Core::String(buffer,len)});
        buffer_ptr = buffer;
    };
    
    char c;
    /// A Boolean to decide whether to continue!
    bool cont = true;
    while(idx != str.size()){
        c = get_char();
        switch (c) {
            // case '\0' : {
            //     cont = false;
            //     break;
            // }
            case '/' : {
                *buffer_ptr = c;
                ++buffer_ptr;
                clear_buffer(Token::Slash);
                break;
            };
            case '.' : {
                *buffer_ptr = c;
                ++buffer_ptr;
                clear_buffer(Token::Dot);
                break;
            }
            case '\\' : {
                *buffer_ptr = c;
                ++buffer_ptr;
                clear_buffer(Token::Slash);
                break;
            };
            case ' ' : {
                break;
            }
            default : {
                if(isalnum(c)){
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    if(!isalnum(ahead_char())){
                        clear_buffer(Token::ID);
                    };
                };
                break;
            }
        };
        ++idx;
    };
    
    
};

Core::String & FSPath::ext(){
    // if(tokens.back().type == Token::ID && tokens[tokens.size() - 2].type == Token::Dot){
    //     return tokens.back().str;
    // }
    return tokens.back().str;
};

Core::String FSPath::filename(){
    Core::String res = "";
    auto it = tokens.begin();
    while(it != tokens.end()){
        auto &type  = it->type;
        if(type == Token::ID){
            ++it;
            auto & type = it->type;
            if(type == Token::Dot){
                res.append((it - 1)->str);
                if(it == (tokens.end() - 2)){
                    return res;
                    break;
                };
                
                while(it != (tokens.end() - 2)){
                    res.append(it->str);
                    ++it;
                };
                return res;
                break;
            };
        };
        ++it;
    };
};

Core::String FSPath::serialize(){
#ifdef TARGET_WIN32
    #define PATH_LIMIT 200
#endif
    std::ostringstream out;
#ifdef TARGET_MACOS
    out << get_asset_dir() << std::flush;
#endif
    
#ifdef TARGET_WIN32
    CHAR buffer[PATH_LIMIT];
    if(GetCurrentDirectoryA(PATH_LIMIT,buffer)){
        out << buffer << std::flush;
    };
#endif
    
    auto it = tokens.begin();
    if(it->type == Token::Dot){
        ++it;
    }
    else if(it->type == Token::ID){
        out << '/' << std::flush;
        ++it;
    };
    
    while(it != tokens.end()){
#ifdef TARGET_WIN32
        if(it->type == Token::Slash){
            out << '\\' << std::flush;
        }
        else {;
#endif
            out << it->str << std::flush;
#ifdef TARGET_WIN32
        };
#endif
        ++it;
    };
    return out.str();
};

FSPath::FSPath(const Core::String & str){
    parse(str);
};

FSPath::~FSPath(){
    
};

}
