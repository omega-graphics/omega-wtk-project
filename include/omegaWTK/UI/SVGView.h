#include "View.h"
#include "omegaWTK/Core/XML.h"

#ifndef OMEGAWTK_UI_SVGVIEW_H
#define  OMEGAWTK_UI_SVGVIEW_H

namespace OmegaWTK {

    typedef Core::XMLDocument SVGDocument;
    class SVGRenderState;
    /**
     @brief Displays the visual output of a SVGSession.
    */
    class OMEGAWTK_EXPORT SVGView : public View {
        SharedHandle<Composition::ViewAnimator> svgAnimator;
    private:
        explicit SVGView(const Core::Rect & rect,Composition::LayerTree *layerTree,ViewPtr parent);
        friend class Widget;
        friend class SVGSession;
    public:
        OMEGACOMMON_CLASS("OmegaWTK.UI.SVGView")
    };

    class OMEGAWTK_EXPORT SVGSession {
        SharedHandle<SVGView> & view;
        SharedHandle<SVGRenderState> currentRenderState;
    private:
        explicit SVGSession(SharedHandle<SVGView> & view);
    public:
        OMEGACOMMON_CLASS("OmegaWTK.UI.SVGSession")
        static SharedHandle<SVGSession> Create(SharedHandle<SVGView> & view);
        void setSVGSource(SVGDocument & document);
        void start();
        void pause();
        void reset();
    };

};

#endif