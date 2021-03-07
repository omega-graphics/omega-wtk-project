#include "BDCompositionDevice.h"
#include "BDCompositionImage.h"

#ifndef OMEGAWTK_COMPOSITION_BDCOMPOSITIONVISUALTREE_H
#define OMEGAWTK_COMPOSITION_BDCOMPOSITIONVISUALTREE_H

namespace OmegaWTK::Composition {
    /**
     A visual layer tree to be rasterized to a view.
     */
    class BDCompositionVisualTree {
    public:
        struct Visual {};
    protected:
        Core::SharedPtr<Visual> root_v;
        Core::Vector<Core::SharedPtr<Visual>> body;
    public:
        #ifdef TARGET_MACOS
        virtual Core::SharedPtr<Visual> makeVisual(Core::SharedPtr<BDCompositionImage> &img) = 0;
        #endif
        #ifdef TARGET_WIN32
        virtual Core::SharedPtr<Visual> makeVisual(Core::SharedPtr<BDCompositionImageRenderTarget> & imgRenderTarget) = 0;
        #endif
        virtual void replaceRootVisual(Core::SharedPtr<Visual> & visual) = 0;
        virtual void replaceVisualWithTargetPtr(Core::SharedPtr<BDCompositionImageRenderTarget> & imgTarget,Core::SharedPtr<Visual> & visual) = 0;
        virtual void setRootVisual(Core::SharedPtr<Visual> & visual) = 0;
        virtual void addVisual(Core::SharedPtr<Visual> &visual) = 0;
    };

};

#endif
