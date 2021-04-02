#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/Math.h"

#ifndef OMEGAWTK_COMPOSITION_GEOMETRICGRAPHICSPATH_H
#define OMEGAWTK_COMPOSITION_GEOMETRICGRAPHICSPATH_H

namespace OmegaWTK::Composition {
class OMEGAWTK_EXPORT  GeometricGraphicsPath {
    Core::Math::VectorPath2D path_a;
    Core::Math::VectorPath2D path_b;
public:
    GeometricGraphicsPath(Core::Math::VectorPath2D & path_a,Core::Math::VectorPath2D & path_b);
};
Core::SharedPtr<GeometricGraphicsPath> RoundedRectFrame(Core::FRoundedRect rect,unsigned width);
Core::SharedPtr<GeometricGraphicsPath> EllipseFrame(Core::FEllipse ellipse,unsigned width);
Core::SharedPtr<GeometricGraphicsPath> Arc(Core::Math::Arc arc,unsigned width);

};

#endif
