#include <thread>
#include <mutex>
#include "Core.h"

#ifndef OMEGAWTK_CORE_MULTITHREADING_H
#define OMEGAWTK_CORE_MULTITHREADING_H

namespace OmegaWTK {

namespace Core {
    template<class _PTy = void>
    OMEGAWTK_EXPORT class CPUThreadP {
        std::thread t;
        
        CPUThread();
    public:
        template<class _FTy,class ..._Args>
        static SharedHandle<CPUThread> OpenWithVoidReturn(){
            
        };
        
        template<class _FTy,class ..._Args>
        static SharedHandle<CPUThread> OpenWithPromiseReturn(){
            
        };
        void close();
        ~CPUThread();
    };

    typedef CPUThreadP <void> CPUThread;

};

};

#endif
