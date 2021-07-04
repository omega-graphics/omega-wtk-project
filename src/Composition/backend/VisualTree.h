#include "../Compositor.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_VISUALTREE_H
#define OMEGAWTK_COMPOSITION_BACKEND_VISUALTREE_H

namespace OmegaWTK::Composition {

    INTERFACE BackendVisualTree {
    protected:
        struct Visual {

        }; 
    public:
        INTERFACE_METHOD(void,addVisual,Core::SharedPtr<Visual> & visual)
        INTERFACE_METHOD(Core::SharedPtr<Visual>,makeVisual,GERenderTargetContext & renderContext)
        INTERFACE_METHOD(void,setRootVisual,Core::SharedPtr<Visual> & visual)     
    };

};

#endif