#include "omegaWTK/Core/FS.h"
#include "FSCocoa.h"

#if defined(TARGET_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif

namespace OmegaWTK::FS {

   

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