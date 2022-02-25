#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_COMPOSITION_PATH_H
#define OMEGAWTK_COMPOSITION_PATH_H

namespace OmegaWTK::Composition {

struct Brush;

class OMEGAWTK_EXPORT  Path {
    struct Segment {
        /// Main Vector Paths of Segment
        OmegaGTE::GVectorPath2D path;
        OmegaGTE::GVectorPath2D final_path_a;
        OmegaGTE::GVectorPath2D final_path_b;
        explicit Segment(OmegaGTE::GPoint2D startPoint);
        bool closed = false;
    };


   OmegaCommon::Vector<Segment> segments;

   unsigned currentStroke;

   float arcPrecision;

    Core::SharedPtr<Brush> pathBrush;


public:
    /// A New Segment is Created
    void goTo(OmegaGTE::GPoint2D pt);

    void setStroke(unsigned newStroke);

    void setPathBrush(Core::SharedPtr<Brush> & brush);

    void setArcPrecision(unsigned newPrecision);

    /// Adds a line to the current segment
    void addLine(OmegaGTE::GPoint2D dest_pt);

    /// Adds an arc to the current segment
    void addArc(OmegaGTE::GRect bounds,float startAngle,float endAngle);

    /// Close current path.
    void close();

    explicit Path(OmegaGTE::GPoint2D start,unsigned initialStroke = 1);
    explicit Path(OmegaGTE::GVectorPath2D & path,unsigned stroke = 1);
    ~Path() = default;
};
Core::SharedPtr<Path> RoundedRectFrame(Core::RoundedRect rect,unsigned width);
Core::SharedPtr<Path> EllipseFrame(Core::Ellipse ellipse,unsigned width);

};

#endif
