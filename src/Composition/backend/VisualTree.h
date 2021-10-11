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
            // BackendRenderTargetContext imgRenderTarget;
            Core::Position pos;
        }; 
        Core::SharedPtr<Visual> root;
        OmegaCommon::Vector<Core::SharedPtr<Visual>> body;
    public:
        INTERFACE_METHOD void addVisual(Core::SharedPtr<Visual> & visual) ABSTRACT;
        INTERFACE_METHOD Core::SharedPtr<Visual> makeVisual(OmegaGTE::NativeRenderTargetDescriptor & targetDesc,
                                                            Core::Position & pos) ABSTRACT;
        INTERFACE_METHOD void setRootVisual(Core::SharedPtr<Visual> & visual) ABSTRACT;    
        INTERFACE_METHOD ~BackendVisualTree() = default;
    };

    // Core::SharedPtr<BackendVisualTree> CreateVisualTree();

};

#endif