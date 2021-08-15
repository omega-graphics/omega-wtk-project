#include "View.h"
#include "omegaWTK/Core/XML.h"

#ifndef OMEGAWTK_UI_SVGVIEW_H
#define  OMEGAWTK_UI_SVGVIEW_H

namespace OmegaWTK {

    typedef Core::XMLDocument SVGDocument; 
    /**
     @brief Displays an SVG Document.
    */
    class SVGView : public View {
    public:
        SVGView(View *parent);
        void setSVGSource(SVGDocument &doc);
    };

};

#endif