#include "omegaWTK/Core/Core.h"
#include <cstring>
#include <algorithm>

namespace OmegaWTK::Core {

bool Rect::compare(Rect & other){
    return (pos.x == other.pos.x) && (pos.y == other.pos.y) && (dimen.minWidth == other.dimen.minWidth) && (dimen.minHeight == other.dimen.minHeight);
};

bool RoundedRect::compare(RoundedRect & other){
    return Rect::compare(other) && (radius_x == other.radius_x) && (radius_y == other.radius_y);
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

}
