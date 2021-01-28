#include <string>
#include <vector>
#include <map>
#include <memory>
#include <queue>
#include <cassert>

#ifndef OMEGAWTK_CORE_CORE_H
#define OMEGAWTK_CORE_CORE_H

namespace OmegaWTK {
    namespace Core {
        typedef unsigned char Option;
    };

#define implements(interfaces...) : public interfaces
#define STATIC_OPT static constexpr Core::Option
#define OPT_PARAM Core::Option
#define ENUM(name,args...) enum class name : Core::Option { args };
#define CORE_CLASS(name) class name

    namespace Core {
    
        typedef std::string String;
    
        template<class _Ty>
        using Vector = std::vector<_Ty>;

        template<class _Ty>
        using Queue = std::queue<_Ty>;

        template<class _Key,class _Val>
        using Map = std::map<_Key,_Val>;

        template<class _Ty>
        using UniquePtr = std::unique_ptr<_Ty>;
        /// Array Reference Class!
        template<class _Ty>
        class ArrayRef {
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

        struct Position {
            unsigned x;
            unsigned y;
        };
        struct Dimensions {
            unsigned minWidth;
            unsigned minHeight;
        };
        struct Rect {
            typedef enum : OPT_PARAM {
                Regular,
                Rounded
            } RectType;
            Position pos;
            Dimensions dimen;
            Rect(Position _pos,Dimensions _dimen):pos(_pos),dimen(_dimen){};
        };
        /// A vector that acts like a queue (first in , first out), but has control over every element and its order in the container.
        template<class _Ty>
        CORE_CLASS(QueueVector) 
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
        #endif
    };
};

#endif
