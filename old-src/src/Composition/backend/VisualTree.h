#include "../Compositor.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_VISUALTREE_H
#define OMEGAWTK_COMPOSITION_BACKEND_VISUALTREE_H

namespace OmegaWTK::Composition {
    /**
     @brief The Interface for rendering LayerTreeLimbs
    */
    INTERFACE BackendVisualTree {
    protected:
        
        struct Visual {
            GERenderTargetContext imgRenderTarget;
            Core::Position pos;
        }; 
        Core::SharedPtr<Visual> root;
        OmegaCommon::Vector<Core::SharedPtr<Visual>> body;
    public:
        INTERFACE_METHOD(void,addVisual,Core::SharedPtr<Visual> & visual)
        INTERFACE_METHOD(Core::SharedPtr<Visual>,makeVisual,GERenderTargetContext & renderContext,
                                                            OmegaGTE::NativeRenderTargetDescriptor & targetDesc,
                                                            Core::Position & pos)
        INTERFACE_METHOD(void,setRootVisual,Core::SharedPtr<Visual> & visual)    
        virtual ~BackendVisualTree(){
            
        }; 
    };

    Core::SharedPtr<BackendVisualTree> CreateVisualTree();

};

#endif