#include <thread>
#include <mutex>
#include <future>
#include <atomic>
#include "Core.h"

#ifndef OMEGAWTK_CORE_MULTITHREADING_H
#define OMEGAWTK_CORE_MULTITHREADING_H

namespace OmegaWTK {

namespace Core {

    class SpinLock {
        std::atomic_bool _l = false;
    public:
        void lock(){
            while(_l.exchange(true,std::memory_order_acquire));
        };
        void unlock(){
            _l.store(false,std::memory_order_release);
        };
        SpinLock(){};
        ~SpinLock(){};
    };

    #define COWPTR_STATUS_READONLY 0x00
    #define COWPTR_STATUS_READWRITE 0x01
    /**
     A smart pointer that implements the Copy on Write algorithim, 
     which allows for thread-safe read/write access for a shared resource.
    */
    template<class _Ty>
    OMEGAWTK_EXPORT class CopyOnWritePtr {
        Core::SharedPtr<_Ty> data;
        std::mutex mutex;
        bool status;
    public:
        /**
         Marks the status of the data.
         @param val The Status..(This MUST be either COWPTR_STATUS_READONLY or COWPTR_STATUS_READWRITE)
        */
        void markStatus(bool val){
            std::lock_guard<std::mutex> lk(mutex);
            status = val;
        };
        /**
         Retrieves the status of the data.
         @returns bool (Limited to COWPTR_STATUS_READONLY or COWPTR_STATUS_READWRITE)
        */
        bool & getStatus(){
            std::lock_guard<std::mutex> lk(mutex);
            return status;
        };
         /** 
            @brief Makes a copy of the object stored and returns a shared handle to it.
            @paragraph 
            The class of the object stored in `data` MUST implement a copy constructor.
            For Example: 
            @code{.cpp}
             
             TestClass::TestClass(TestClass & other){
                ///Implement copy constructor here.
             };

            @endcode
            */
        Core::SharedPtr<_Ty> makeCopy(){
            std::lock_guard<std::mutex> lk(mutex);
            return std::shared_ptr<_Ty>(::new _Ty(*data));
        };
        CopyOnWritePtr(_Ty *data):data(data),status(COWPTR_STATUS_READWRITE){

        };
    };

    
    template<class _Ty,class ..._Args>
    OMEGAWTK_EXPORT CopyOnWritePtr<_Ty> make_copy_on_write(_Ty && t,_Args && ...args){
        static_assert(std::is_constructible_v<_Ty,_Args...>,"This item is not constructible");
        return CopyOnWritePtr<_Ty>(new _Ty(std::forward<_Args>(args)...));
    };
    

    template<class _PTy = void>
    OMEGAWTK_EXPORT class CPUThreadP {
        std::thread t;
        std::promise<_PTy> prom;
        std::future<_PTy> future;
        std::mutex mutex;
        /// Function Type must have the first argument be equal to a pointer to the parent CPUThreadP
        template<class _FTy,class ..._Args>
        CPUThreadP(_FTy f,_Args ...args):t(f,this,args...),future(prom.get_future()){
            
        };
    public:
        /// Please only call this in the actual thread...
        std::promise<_PTy> & promise(){
            std::lock_guard<std::mutex> lk(mutex);
            return prom;
        };
        _PTy waitForPromise() throw() {
            return future.get();
        };
        template<class _FTy,class ..._Args>
        static SharedHandle<CPUThreadP> OpenWithVoidReturn(_FTy f,_Args ...args){
            return std::shared_ptr<CPUThreadP>(new CPUThreadP<>(f,args...));
        };
        
        template<class _IPTy,class _FTy,class ..._Args>
        static SharedHandle<CPUThreadP> OpenWithPromiseReturn(_FTy f,_Args ...args){
            return std::shared_ptr<CPUThreadP>(new CPUThreadP<_IPTy>(f,args...));
        };
        void close(){
            t.join();
        };
        ~CPUThreadP(){
           close();
        };
    };

    typedef CPUThreadP <> CPUThread;

};

};

#endif
