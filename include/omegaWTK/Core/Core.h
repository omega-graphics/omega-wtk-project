#include <string>
#include <vector>
#include <map>
#include <memory>
#include <queue>
#include <cassert>
#include <list>
#include <optional>
#include <sstream>
#include <fstream>
#include "OmegaWTKExport.h"

#ifdef TARGET_WIN32
#include <wrl.h>
#pragma comment(lib,"runtimeobject.lib")
#endif

#ifndef OMEGAWTK_CORE_CORE_H
#define OMEGAWTK_CORE_CORE_H

namespace OmegaWTK {
    namespace Core {
        typedef unsigned char Option;
    };

#define STATIC_OPT static constexpr Core::Option
#define OPT_PARAM Core::Option

    namespace Core {
    
        typedef std::string String;
    
        template<class _Ty>
        using Vector = std::vector<_Ty>;

        template<class _Ty>
        using List = std::list<_Ty>;

        template<class _Ty>
        using Queue = std::queue<_Ty>;

        template<class _Key,class _Val>
        using Map = std::map<_Key,_Val>;

        template<class _Ty>
        using UniquePtr = std::unique_ptr<_Ty>;
    
        template<class _Ty>
        using SharedPtr = std::shared_ptr<_Ty>;

        template<class _Ty>
        using Optional = std::optional<_Ty>;
    
        typedef std::istream IStream;
        typedef std::ostream OStream;
        /// Array Reference Class!
        template<class _Ty>
        class OMEGAWTK_EXPORT ArrayRef {
            _Ty *_data;
            unsigned _size;
        public:
            using iterator = _Ty *;
            using const_iterator = const _Ty *;
            using reference  = _Ty &;
            const _Ty * data(){
                return _data;
            };
            const unsigned & size(){
                return _size;
            };
            iterator begin(){
                return _data;
            };
            iterator end(){
                return _data + (sizeof(_Ty) * _size);
            };
            reference operator[](unsigned idx){
                return begin()[idx];
            };
            ArrayRef() = delete;
            ArrayRef(Core::Vector<_Ty> & ref):_data(ref.data()),_size(ref.size()){};
        };

        struct OMEGAWTK_EXPORT Position {
            unsigned x;
            unsigned y;
        };
        struct OMEGAWTK_EXPORT Dimensions {
            unsigned minWidth;
            unsigned minHeight;
        };
        struct OMEGAWTK_EXPORT Rect {
            Position pos;
            Dimensions dimen;
            bool compare(Rect & other);
            float angle;
            Rect(Position _pos,Dimensions _dimen,float angle = 0):pos(_pos),dimen(_dimen),angle(angle){};
        };
    
        struct OMEGAWTK_EXPORT RoundedRect : public Rect {
            unsigned radius_x;
            unsigned radius_y;
            bool compare(RoundedRect & other);
            RoundedRect(const Rect &rect,unsigned rad_x,unsigned rad_y):Rect(rect),radius_x(rad_x),radius_y(rad_y){};
        };
    
        struct OMEGAWTK_EXPORT Ellipse {
            Position pos;
            unsigned radius_x;
            unsigned radius_y;
            bool compare(Ellipse & other);
        };
        struct OMEGAWTK_EXPORT FPosition {
            float x;
            float y;
        };
        struct OMEGAWTK_EXPORT FDimensions {
            float minWidth;
            float minHeight;
        };
        struct OMEGAWTK_EXPORT FRect {
            FPosition pos;
            FDimensions dimen;
            float angle;
            FRect(FPosition pos,FDimensions dimen,float angle = 0):pos(pos),dimen(dimen),angle(angle){};
        };
        struct OMEGAWTK_EXPORT FRoundedRect : public FRect {
            float radius_x;
            float radius_y;
            FRoundedRect(const FRect & rect,float radius_x,float radius_y):FRect(rect),radius_x(radius_x),radius_y(radius_y){};
        };
        struct OMEGAWTK_EXPORT FEllipse {
            FPosition pos;
            float radius_x;
            float radius_y;
        };
        /// A vector that acts like a queue (first in , first out), but has control over every element and its order in the container.
        template<class _Ty>
        class OMEGAWTK_EXPORT QueueVector
        {
            _Ty *_data;
            public:
            using size_type = unsigned;
            private:
            size_type len = 0;
            public:
            using iterator = _Ty *;
            using reference = _Ty &;
            const size_type & size() noexcept {return len;};
            bool empty() noexcept {return len == 0;};
            iterator begin(){ return _data;};
            iterator end(){return _data + (len * sizeof(_Ty));};
            reference first(){ return begin()[0];};
            reference last(){ return end()[-1];};
            reference operator[](size_type idx){ return begin()[idx];};
            private:
            void _push_el(const _Ty & el){
                if(len == 0)
                    _data = new _Ty(std::move(el));
                else {
                    _Ty temp[len];
                    std::move(begin(),end(),temp);
                    delete [] _data;
                    _data = new _Ty[len + 1];
                    std::move(temp,temp + (sizeof(_Ty) * len),begin());
                    begin()[len] = std::move(el);
                };
                ++len;
            }; 
            void _insert_el_at_idx(const _Ty & el,size_type & idx){
                if(len == 0) {
                    assert(idx == 0 && "Cannot emplace item at requested index! No mem allocated!");
                    _data = new _Ty(std::move(el));
                }
                else {
                    assert(idx < len && "Index is out of range!");
                    _Ty temp[len + 1];
                    std::move(begin(),begin() + (idx * sizeof(_Ty)),temp);
                    temp[idx] = std::move(el);
                    std::move(begin() + (idx * sizeof(_Ty)),end(),temp + ((idx+1) * sizeof(_Ty)));
                    delete [] _data;
                    _data = new _Ty[len + 1];
                    std::move(temp,temp + (sizeof(_Ty) * (len + 1)),begin());
                };
                ++len;
            };
            public:
            void insert(const _Ty & el,size_type idx){
                _insert_el_at_idx(el,idx);
            };
            void insert(_Ty && el,size_type idx){
                _insert_el_at_idx(el,idx);
            };
            void push(const _Ty & el){
                _push_el(el);
            };
            void push(_Ty && el){
                _push_el(el);
            };
            void pop(){
                assert(!empty() && "Cannot call pop() on empty QueueVector!");
                auto f_el = first();
                f_el.~_Ty();
                _Ty temp[len-1];
                std::move(begin() + sizeof(_Ty),end(),temp);
                delete [] _data;
                --len;
                _data = new _Ty[len];
                std::move(temp,temp + (len * sizeof(_Ty)),begin());
            };
            QueueVector():_data(nullptr),len(0){};
            QueueVector(const QueueVector<_Ty> & other):len(other.len){
                _data = new _Ty[len];
                std::copy(other.begin(),other.end(),begin());
            };
            QueueVector(QueueVector<_Ty> && other):len(other.len){
                _data = new _Ty[len];
                std::copy(other.begin(),other.end(),begin());

            };
            ~QueueVector(){
                auto it = begin();
                while(it != end()){
                    reference item = *it;
                    item.~_Ty();
                    ++it;
                };
                delete [] _data;
            };

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
        #ifdef TARGET_WIN32
        template <class T> void SafeRelease(T **ppT)
        {
            if (*ppT)
            {
                (*ppT)->Release();
                *ppT = NULL;
            }
        }            
        /// A ComPtr that releases its object on its destruction. (Similar to the std::unique_ptr)
        template<class T>
        class UniqueComPtr {
        public:
            Microsoft::WRL::ComPtr<T> comPtr;
            T * get() { return comPtr.Get();};
            T * operator->(){
                return comPtr.Get();
            };
            T ** operator&(){
                return comPtr.GetAddressOf();
            };
            UniqueComPtr() = default;
            // UniqueComPtr(Microsoft::WRL::ComPtr<T> _com):comPtr(_com){};
            UniqueComPtr(T *ptr):comPtr(ptr){};
            ~UniqueComPtr(){
                auto ptr = comPtr.Detach();
                Core::SafeRelease(&ptr);
            };
        };
        #endif
    };
    Core::Rect Rect(unsigned x,unsigned y,unsigned width,unsigned height,float angle = 0);
    Core::Ellipse Ellipse(unsigned x,unsigned y,unsigned radius_x,unsigned radius_y);
    Core::RoundedRect RoundedRect(unsigned x,unsigned y,unsigned width,unsigned height,unsigned radius_x,unsigned radius_y,float angle = 0);
     
    Core::FRect FRect(float x,float y,float w,float h,float angle = 0);
    Core::FEllipse FEllipse(float x,float y,float rad_x,float rad_y);
    Core::FRoundedRect FRoundedRect(float x,float y,float w,float h,float rad_x,float rad_y,float angle = 0);


    class FSPath {
        struct Token {
            typedef enum : OPT_PARAM {
                ID,
                Dot,
                Slash,
            } Type;
            Type type;
            Core::String str;
        };
        Core::Vector<Token> tokens;
        void parse(const Core::String & str);
    public:
        const unsigned getTokenCount(){ return tokens.size();};
        Core::String debugString(){
            std::ostringstream out;
            auto it = tokens.begin();
            while(it != tokens.end()){
                out << "{Type:" << int(it->type) << ",Content:" << it->str << "}, " << std::flush;
                ++it;
            };
            return out.str();
        };
        // Core::String dir();
        Core::String filename();
        Core::String & ext();
        Core::String serialize();
        FSPath(const Core::String & str);
        ~FSPath();
    };
};

template<class _Ty>
OMEGAWTK_EXPORT using SharedHandle = OmegaWTK::Core::SharedPtr<_Ty>;
/**Creates a Shared Instance of _Ty and returns it*/
template<class _Ty,class... _Args>
OMEGAWTK_EXPORT inline SharedHandle<_Ty> make(_Args && ...args){
//    static_assert(std::is_constructible<_Ty,_Args...>::value,"Cannot construct item");
    return std::make_shared<_Ty>(args...);
};
template<class _Ty>
OMEGAWTK_EXPORT using UniqueHandle = OmegaWTK::Core::UniquePtr<_Ty>;
/**Creates a Unique Instance of _Ty and returns it*/
template<class _Ty,class... _Args>
OMEGAWTK_EXPORT inline UniqueHandle<_Ty> && construct(_Args && ...args){
    static_assert(std::is_constructible<_Ty,_Args...>::value,"Cannot construct item");
    return std::make_unique<_Ty>(args...);
};

#endif
