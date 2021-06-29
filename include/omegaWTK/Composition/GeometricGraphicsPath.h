#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_GEOMETRICGRAPHICSPATH_H
#define OMEGAWTK_COMPOSITION_GEOMETRICGRAPHICSPATH_H

namespace OmegaWTK::Composition {
class OMEGAWTK_EXPORT  GeometricGraphicsPath {
    OmegaGTE::GVectorPath2D path_a;
    OmegaGTE::GVectorPath2D path_b;
public:
    GeometricGraphicsPath(OmegaGTE::GVectorPath2D & path_a,OmegaGTE::GVectorPath2D & path_b);
};
Core::SharedPtr<GeometricGraphicsPath> RoundedRectFrame(Core::RoundedRect rect,unsigned width);
Core::SharedPtr<GeometricGraphicsPath> EllipseFrame(Core::Ellipse ellipse,unsigned width);
Core::SharedPtr<GeometricGraphicsPath> Arc(OmegaGTE::GArc arc,unsigned width);

};

#endif
