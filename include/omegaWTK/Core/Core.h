#include <omega-common/common.h>

#include <OmegaGTE.h>

/// Regex Lib
// #define PCRE2_CODE_UNIT_WIDTH 8
// #include <pcre2.h>

#include "OmegaWTKExport.h"
#include "Unicode.h"

#ifdef TARGET_WIN32
#include <wrl.h>
#pragma comment(lib,"runtimeobject.lib")
#endif

#ifndef OMEGAWTK_CORE_CORE_H
#define OMEGAWTK_CORE_CORE_H

namespace OmegaWTK {

    #define INTERFACE class

    #define INTERFACE_METHOD(type,name,args...) virtual type name(args) = 0;

    #define DELEGATE INTERFACE
    
    #define DELEGATE_METHOD(type,name,args...) INTERFACE_METHOD(type,name,args)
    
    typedef enum : int {
        CodeOk,
        CodeFailed
    } StatusCode;



    namespace Core {
         typedef unsigned char Option;
    };

#define STATIC_OPT static constexpr Core::Option
#define OPT_PARAM Core::Option

    namespace Core {

        template<class _Ty>
        using UniquePtr = std::unique_ptr<_Ty>;
    
        template<class _Ty>
        class OMEGAWTK_EXPORT UniquePtrRef {
            UniquePtr<_Ty> & ptr;
        public:
            bool hasExpired(){
                return ptr == nullptr;
            };
            void resetRef(UniquePtr<_Ty> & _new_ptr){
                ptr = _new_ptr;
            };
            UniquePtrRef(UniquePtr<_Ty> & _ptr):ptr(_ptr){};
        };
    
        template<class _Ty>
        using SharedPtr = std::shared_ptr<_Ty>;
    
        template<class _Ty>
        using WeakPtr = std::weak_ptr<_Ty>;

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
            ArrayRef(OmegaCommon::Vector<_Ty> & ref):_data(ref.data()),_size(ref.size()){};
        };


      
        /// A vector that acts like a queue (first in , first out), but has control over every element and its order in the container.
        template<class _Ty>
        class OMEGAWTK_EXPORT   QueueVector
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
        /** @brief A queue data type that preallocates its memory on the heap that has a limited capacity, however it can be resized when nesscary.
            @paragraph
             This class typically gets used in a scenario
             where there could be thousands of objects that get dynamically constructed and destroyed by a standard data type such as std::vector 
             but in a scenario as such, all the standard types are extremely inefficient and can cause fragmented memory. 

             The QueueHeap class has a similar implementation to a heap data type rather it has more control over how the data gets copied to/removed from the heap. 
             In addition, it only allows construction/destruction of objects in the notions of a "first in, first out" data type.
        */
        template<class _Ty>
        class QueueHeap {
            protected:
            _Ty *_data;
            public:
            using size_type = unsigned;
            private:
            size_type len = 0;
            size_type max_len;
            public:
            using reference = _Ty &;
            bool empty() noexcept {return len == 0;};
            bool full() noexcept {return len == max_len;};
            size_type & length(){ return len;};
            reference first(){ return _data[0];};
            reference last(){ return _data[len-1];};
            protected:
            void __push_el(const _Ty & el){
                memcpy(_data + len,&el,sizeof(_Ty));
                ++len;
            };
            public:
            virtual void push(const _Ty & el){
                __push_el(el);
            };
            virtual void push(_Ty && el){
                __push_el(el);
            };
            void pop(){
                assert(!empty() && "Cannot call pop() on empty QueueHeap!");
                first().~_Ty();
                --len;
                memcpy(_data,_data + 1,sizeof(_Ty) * len);
            };
            void resize(size_type new_max_size){
                assert(max_len < new_max_size && "");
                realloc(_data,sizeof(_Ty) * new_max_size);
                max_len = new_max_size;
            };

            QueueHeap(size_type max_size):_data((_Ty *)malloc(sizeof(_Ty) * max_size)),max_len(max_size){

            };
            ~QueueHeap(){
                while(!empty())
                    pop();
                free(_data);
            };
        };

        template<class _Ty,class Compare_Ty>
        class PriorityQueueHeap : public QueueHeap<_Ty> {
            Compare_Ty comp;
            using super = QueueHeap<_Ty>;
            void _sort(){
                std::sort(super::_data,super::_data + this->length(),comp);
            };
            public:
            void push(const _Ty & el) override{
                super::__push_el(el);
                _sort();
            };
            void push(_Ty && el) override{
                super::__push_el(el);
                _sort();
            };
            
            PriorityQueueHeap(typename super::size_type max_size,Compare_Ty comp = Compare_Ty()):QueueHeap<_Ty>(max_size),comp(comp){

            };
            ~PriorityQueueHeap(){
                
            };
        };

        typedef OmegaGTE::GRect Rect;

        typedef OmegaGTE::GPoint2D Position;

        typedef OmegaGTE::GRoundedRect RoundedRect;

        struct OMEGAWTK_EXPORT Ellipse : OmegaGTE::GEllipsoid {
            float & x;
            float & y;
            float & rad_x;
            float & rad_y;

            Ellipse(float x,float y,float rad_x,float rad_y):
            GEllipsoid({x,y,0,rad_x,rad_y,0}),
            x(OmegaGTE::GEllipsoid::x),
            y(OmegaGTE::GEllipsoid::y),
            rad_x(OmegaGTE::GEllipsoid::rad_x),
            rad_y(OmegaGTE::GEllipsoid::rad_y){
            };
        };




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

        // class OMEGAWTK_EXPORT RegularExpression {
        //     pcre2_code *code;
        // public:
        //     RegularExpression(String pattern,bool multiLine = true);
            
        //     struct Match {
        //         pcre2_match_data *mdata;
        //     public:
        //         String main;
        //         String getSubMatchByNum(unsigned n);
        //         ~Match();
        //     };
        //     Match match(String subject);
        //     ~RegularExpression();
        // };

        // typedef RegularExpression Regex;
    };

    void loadAssetFile(OmegaCommon::FS::Path path);

     template<class _Ty>
    class StatusWithObj {
        StatusCode code;
        _Ty * data;
        const char * message;
        void _construct(const _Ty & obj){
            code = CodeOk;
            data = ::new _Ty;
            memmove(data,&obj,sizeof(_Ty));
        };

    public:
        operator bool(){
            return code == CodeOk;
        };
        StatusCode getCode(){ return code;};
        const char * getError(){ return message;};
        Core::SharedPtr<_Ty> getValue(){
            auto transfer = data;
            data = nullptr;
            return std::make_shared<_Ty>(std::move(*transfer));
        };
        StatusWithObj(const _Ty & obj):message(nullptr){
            _construct(obj);
        };

        StatusWithObj(_Ty && obj):message(nullptr){
            _construct(obj);
        };

        StatusWithObj(const char * message):data(nullptr){
            auto len = strlen(message);
            this->message = new char[len];
            std::move((char *)message,(char *)message + len,this->message);
            code = CodeFailed;
        };
        ~StatusWithObj(){
            // if(data != nullptr){
            //     data->~_Ty();
            //     delete data;
            // }
            if(message != nullptr) {
                delete message;
            };
        };
    };

};

template<class _Ty>
using SharedHandle = OmegaWTK::Core::SharedPtr<_Ty>;
/**Creates a Shared Instance of _Ty and returns it*/
template<class _Ty,class... _Args>
inline SharedHandle<_Ty> make(_Args && ...args){
//    static_assert(std::is_constructible<_Ty,_Args...>::value,"Cannot construct item");
    return std::make_shared<_Ty>(args...);
};
template<class _Ty>
using UniqueHandle = OmegaWTK::Core::UniquePtr<_Ty>;
/**Creates a Unique Instance of _Ty and returns it*/
template<class _Ty,class... _Args>
inline UniqueHandle<_Ty> && construct(_Args && ...args){
    static_assert(std::is_constructible<_Ty,_Args...>::value,"Cannot construct item");
    return std::make_unique<_Ty>(args...);
};

//template<class _Ty>
//OMEGAWTK_EXPORT using WeakHandle = OmegaWTK::Core::WeakPtr<_Ty>;

#endif
