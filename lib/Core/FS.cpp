#include "omegaWTK/Core/FS.h"
#include "FSCocoa.h"

#if defined(TARGET_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

namespace OmegaWTK::FS {

    void Path::parse(const Core::String & str){

    // Core::Regex regex(R"(([\w|_|\/|\.]*)\/(\w+)(?:\.(\w+))?$)");

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
    
    auto isAlnumAndOther = [&](char c){
        return isalnum(c) ||  (c == '-') || (c == '_');
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
                if(isAlnumAndOther(c)){
                    *buffer_ptr = c;
                    ++buffer_ptr;
                    if(!isAlnumAndOther(ahead_char())){
                        clear_buffer(Token::ID);
                    };
                };
                break;
            }
        };
        ++idx;
    };
    
    
};

Core::String & Path::ext(){
    // if(tokens.back().type == Token::ID && tokens[tokens.size() - 2].type == Token::Dot){
    //     return tokens.back().str;
    // }
    return tokens.back().str;
};

Core::String Path::filename(){
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
    return "";
};

Core::String & Path::str(){
    return _str;
};

bool Path::exists(){
#ifdef TARGET_WIN32
    return PathFileExistsA(absPath().c_str()) == TRUE;
#endif
    #ifdef TARGET_MACOS
        return file_exists(absPath().c_str());
    #endif
};

Core::String Path::absPath(){
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



FS::Path::Path(const Core::String & str){
    _str = str;
    parse(str);
};

FS::Path::Path(const char * str){
    _str = str;
    parse(str);
};

FS::Path::~Path(){
    
};


bool isFile(const Path &path){
    
};
    
bool isDirectory(const Path &path){

};


};