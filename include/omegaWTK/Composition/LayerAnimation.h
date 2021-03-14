#include "omegaWTK/Core/Math.h"

#ifndef OMEGAWTK_COMPOSITION_LAYERANIMATION_H
#define OMEGAWTK_COMPOSITION_LAYERANIMATION_H

namespace OmegaWTK::Composition {
    class Layer;
    /**
     The standard class template for defining a curve-based animation 
     for any value (associated with a certain composition layer) of type `_Val_Ty` 
     over a period of time of type `_Time_Ty`
    */
    template<class _Val_Ty,class _Time_Ty = float>
    class OMEGAWTK_EXPORT LayerAnimation {
        /**
         The Layer associated with this LayerAnimation
        */
        Layer *layer;
    public:
        enum class SegmentType : OPT_PARAM {
            Linear,
            CubicBezier,
            QuadraticBezier
        };
    private:
        struct Segment {
            SegmentType type;
            struct Pt {
                _Time_Ty x;
                _Val_Ty y;
            };
            Core::UniquePtr<Pt> start;
            Core::UniquePtr<Pt> end;

            Core::Optional<Core::UniquePtr<Pt>> controlPt;
            /** 
            Control Points used for CubicBezier Segments
            */
            Core::Optional<Core::UniquePtr<Pt>> controlPtA;
            Core::Optional<Core::UniquePtr<Pt>> controlPtB;
        };
        Core::Vector<Segment> segments;
    public:
        /// Segment Descriptors

        struct SegmentDescriptor {
            _Time_Ty deltaX;
            _Val_Ty deltaY;
        };
        typedef SegmentDescriptor LinearSegmentDescriptor;
        struct CubicBezierSegmentDescriptor : public SegmentDescriptor {
            /// Coordinates of the Control Points from the segment`s start point.
            _Time_Ty _delta_control_pt_a_x;
            _Val_Ty _delta_control_pt_a_y;
            _Time_Ty _delta_control_pt_b_x;
            _Val_Ty _delta_control_pt_b_y;
        };
        struct QuadraticBezierSegmentDescriptor : public SegmentDescriptor {
            /// Coordinates of the Control Point from the segment`s start point.
            _Time_Ty _delta_control_pt_x;
            _Val_Ty _delta_control_pt_y;
        };
        template<class _Ty,std::enable_if_t<std::is_base_of_v<SegmentDescriptor,_Ty>,int> = 0>
        Core::SharedPtr<_Ty> makeDescriptor(){
            return std::make_shared<_Ty>();
        };
        void addLinearSegment(Core::SharedPtr<LinearSegmentDescriptor> & desc);
        void addQuadraticBezierSegment(Core::SharedPtr<QuadraticBezierSegmentDescriptor> & desc);
        void addCubicBezierSegment(Core::SharedPtr<CubicBezierSegmentDescriptor> & desc);
    };
};

#endif