#include "omegaWTK/Core/Core.h"
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>

#include "assetc.h"
#include "AssetsPriv.h"



#ifdef TARGET_WIN32
#include <windows.h>
#include <Shlwapi.h>
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

    // RegularExpression::RegularExpression(Core::String pattern,bool multiLine){
    //     int errc;
    //     size_t err_offset;
    //     uint32_t extra;
    //     if(multiLine)
    //         extra =  PCRE2_MULTILINE;
    //     else 
    //         extra = 0;
    //     code = pcre2_compile(PCRE2_SPTR(pattern.c_str()),pattern.size(),PCRE2_UTF | extra,&errc,&err_offset,NULL);
    //     if(errc >= 0){
    //         std::cout << "Regex Successfuly Compiled" << std::endl;
    //     }
    //     else {
    //         std::cerr << "Failed to Compile Regex" << std::endl;
    //     };
    // };

    // RegularExpression::Match RegularExpression::match(Core::String subject){
    //     pcre2_match_data *data = pcre2_match_data_create_from_pattern(code,NULL);
    //     auto errc = pcre2_match_8(code,PCRE2_SPTR(subject.c_str()),subject.size(),0,PCRE2_COPY_MATCHED_SUBJECT,data,NULL);
    //     String rc;
    //     Match m;
    //     rc.reserve(pcre2_get_ovector_count(data));
    //     memcpy(rc.data(),pcre2_get_ovector_pointer(data),pcre2_get_match_data_size(data));
    //     m.mdata = data;
    //     m.main = std::move(rc);
    //     return m;
    // };

    // String RegularExpression::Match::getSubMatchByNum(unsigned int n){
    //     PCRE2_UCHAR8 *buffer;
        
    //     size_t size;
    //     pcre2_substring_get_bynumber(mdata,n,&buffer,&size);
    //     return String((const char *)buffer,size);
    // };

    // RegularExpression::Match::~Match(){
    //     pcre2_match_data_free(mdata);
    // };

    // RegularExpression::~RegularExpression(){
    //     pcre2_code_free(code);
    // };

}

namespace OmegaWTK {

typedef unsigned char Byte;

Core::Map<Core::String,AssetFileLoader::AssetBuffer>  AssetFileLoader::assets_res;


void AssetFileLoader::loadAssetFile(FSPath & path){
        auto str = path.serialize();
        std::ifstream in(str,std::ios::binary | std::ios::in);
        assetc::AssetsFileHeader header;
        in.read((char *)&header,sizeof(assetc::AssetsFileHeader));
        // MessageBoxA(GetDesktopWindow(),(std::string("Asset Count:") + std::to_string(header.asset_count)).c_str(),"NOTE", MB_OK);
        unsigned i = 0;
        while(i < header.asset_count){
            assetc::AssetsFileEntry fentry;
            in.read((char *)&fentry,sizeof(assetc::AssetsFileEntry));
            /// Read/Buffer the Asset Name
            Byte * name = new Byte[fentry.string_size];
            in.read((char *)name,fentry.string_size);

            /// Read/Buffer the Asset Data
            Byte * data = new Byte[fentry.file_size];
            in.read((char *)data,fentry.file_size);

            std::string filename ((char *)name,fentry.string_size);
            AssetBuffer buffer;
            buffer.filesize = fentry.file_size;
            buffer.data = data;
            assets_res.insert(std::make_pair(std::move(filename),std::move(buffer)));
            i += 1;
        };
        // MessageBoxA(GetDesktopWindow(),"Read Assets File","NOTE", MB_OK);

        in.close();
    };

void loadAssetFile(FSPath path){
    AssetFileLoader::loadAssetFile(path);
};


Core::Rect Rect(unsigned x,unsigned y,unsigned width,unsigned height,float angle){
    return {{x,y},{width,height},angle};
};
Core::Ellipse Ellipse(unsigned x,unsigned y,unsigned radius_x,unsigned radius_y){
    return {{x,y},radius_x,radius_y};
};

Core::RoundedRect RoundedRect(unsigned x,unsigned y,unsigned width,unsigned height,unsigned radius_x,unsigned radius_y,float angle) {
    return {Rect(x,y,width,height,angle),radius_x,radius_y};
}

Core::FRect FRect(float x,float y,float w,float h,float angle){
    return {{x,y},{w,h},angle};
};

Core::FEllipse FEllipse(float x,float y,float rad_x,float rad_y){
    return {{x,y},rad_x,rad_y};
};

Core::FRoundedRect FRoundedRect(float x,float y,float w,float h,float rad_x,float rad_y,float angle){
    return {FRect(x,y,w,h,angle),rad_x,rad_y};
};


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
    return "";
};

Core::String & FSPath::str(){
    return _str;
};

bool FSPath::exists(){
#ifdef TARGET_WIN32
    return PathFileExistsA(serialize().c_str()) == TRUE;
#endif
    #ifdef TARGET_MACOS
        return file_exists(serialize().c_str());
    #endif
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
    _str = str;
    parse(str);
};

FSPath::~FSPath(){
    
};

}
