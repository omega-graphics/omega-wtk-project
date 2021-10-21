#include "View.h"
#include "omegaWTK/Core/XML.h"

#ifndef OMEGAWTK_UI_SVGVIEW_H
#define  OMEGAWTK_UI_SVGVIEW_H

namespace OmegaWTK {

    typedef Core::XMLDocument SVGDocument; 
    /**
     @brief Displays the visual output of a SVGSession.
    */
    class OMEGAWTK_EXPORT SVGView : public View {
        explicit SVGView(const Core::Rect & rect,Composition::LayerTree *layerTree,View * parent);
        friend class Widget;
    };

    class OMEGAWTK_EXPORT SVGSession {
        SharedHandle<SVGView> & view;
    private:
        explicit SVGSession(SharedHandle<SVGView> & view);
    public:
        static SharedHandle<SVGSession> Create(SharedHandle<SVGView> & view);
        void setSVGSource(const SVGDocument & document);
        void start();
        void pause();
        void reset();
    };

};

#endif