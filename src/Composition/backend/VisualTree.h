#include "../Compositor.h"
#include "RenderTarget.h"

#ifndef OMEGAWTK_COMPOSITION_BACKEND_VISUALTREE_H
#define OMEGAWTK_COMPOSITION_BACKEND_VISUALTREE_H

namespace OmegaWTK::Composition {
    /**
     @brief The Interface for rendering LayerTreeLimbs
    */
    INTERFACE BackendVisualTree {
    protected:
        
        struct Visual {
            Core::Position pos;
            BackendRenderTargetContext renderTarget;
            explicit Visual(Core::Position & pos,BackendRenderTargetContext & renderTarget):
            pos(pos),
            renderTarget(renderTarget){

            }
            virtual void resize(Core::Rect & newRect) = 0;
            virtual ~Visual() = default;
        };
        Core::SharedPtr<Visual> root = nullptr;
        OmegaCommon::Vector<Core::SharedPtr<Visual>> body;
    public:
        bool hasRootVisual(){
            return (bool)root;
        };
        static SharedHandle<BackendVisualTree> Create(SharedHandle<ViewRenderTarget> & view);
        INTERFACE_METHOD void addVisual(Core::SharedPtr<Visual> & visual) ABSTRACT;
        INTERFACE_METHOD Core::SharedPtr<Visual> makeVisual(Core::Rect & rect,Core::Position & pos) ABSTRACT;
        INTERFACE_METHOD void setRootVisual(Core::SharedPtr<Visual> & visual) ABSTRACT;    
        INTERFACE_METHOD ~BackendVisualTree() = default;
    };

    

};

#endif