#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/MultiThreading.h"

#ifndef OMEGAWTK_COMPOSITION_LAYERANIMATION_H
#define OMEGAWTK_COMPOSITION_LAYERANIMATION_H

namespace OmegaWTK {
    class ViewAnimator;
};

namespace OmegaWTK::Composition {
    class Layer;
//    class ViewRenderTargetFrameScheduler;

    /**
     @interface
     The standard class template for defining a curve-based animation
     for any value (associated with a certain composition layer) of type `_Val_Ty` 
     over a period of time of type `_Time_Ty`
     
    */
    template<typename _Val_Ty,typename _Time_Ty>
    class OMEGAWTK_EXPORT LayerAnimation_Base {
        friend class LayerAnimationController;
    public:
        enum class SegmentType : OPT_PARAM {
            Linear,
            CubicBezier,
            QuadraticBezier
        };
    protected:
        _Val_Ty * val;
        /**
         The Layer associated with this LayerAnimation
        */
        Layer *layer;
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
        void _computeYValWithX(_Time_Ty & x,Segment & seg){
            /// X can only be betwee 0 and 1.0 for this segment
            switch (seg.type) {
                case SegmentType::Linear : {
                    auto & Pt_A = *(seg.start);
                    auto & Pt_B = *(seg.end);
                    
                    auto m = (Pt_B.y - Pt_A.y)/(Pt_B.x - Pt_A.x);
                    _Val_Ty new_val = x * m;
                    *(this->val) = new_val;
                    
                    break;
                }
                case SegmentType::CubicBezier : {
                    
                    break;
                }
                case SegmentType::QuadraticBezier : {
                    
                    break;
                }
                default:
                    break;
            }
        };
        /**
         Modifies layer value by deltaY, based on deltaX provided.
         */
        virtual typename Segment::Pt & getFirstPoint() = 0;
        virtual typename Segment::Pt & getLastPoint() = 0;
         virtual void traverseCurve(_Time_Ty x) = 0;
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
        template<class _Ty,std::enable_if_t<std::is_base_of_v<::OmegaWTK::Composition::LayerAnimation_Base<_Val_Ty,_Time_Ty>::SegmentDescriptor,_Ty>,int> = 0>
        Core::SharedPtr<_Ty> makeDescriptor(){
            return std::make_shared<_Ty>();
        };
        LayerAnimation_Base(Layer *_layer,_Val_Ty * val):layer(_layer),val(val){};
    };

    template<typename _Val_Ty,typename _Time_Ty>
    class OMEGAWTK_EXPORT  LayerAnimation_Impl : public LayerAnimation_Base<_Val_Ty,_Time_Ty> {
        typedef LayerAnimation_Base<_Val_Ty,_Time_Ty> PARENT;
        OmegaCommon::Vector<typename PARENT::Segment> segments;
    public:
        typename PARENT::Segment::Pt & getFirstPoint(){
            return *(segments.front().start);
        };
        typename PARENT::Segment::Pt & getLastPoint(){
            return *(segments.back().end);
        };
        void traverseCurve(_Time_Ty x){
            
        };
        void addLinearSegment(Core::SharedPtr<typename PARENT::LinearSegmentDescriptor> & desc);
        void addQuadraticBezierSegment(Core::SharedPtr<typename PARENT::QuadraticBezierSegmentDescriptor> & desc);
        void addCubicBezierSegment(Core::SharedPtr<typename PARENT::CubicBezierSegmentDescriptor> & desc);
        LayerAnimation_Impl(Layer *layer,_Val_Ty * val):LayerAnimation_Base<_Val_Ty,_Time_Ty>(layer,val){};
    };

    template<typename _Val_Ty,typename _Time_Ty>
    class OMEGAWTK_EXPORT  LayerTransition_Impl : public LayerAnimation_Base<_Val_Ty,_Time_Ty> {
        typedef LayerAnimation_Base<_Val_Ty,_Time_Ty> PARENT;
        typename PARENT::Segment segment;
    public:
        typename PARENT::Segment::Pt & getFirstPoint(){
            return *(segment.first);
        };
        typename PARENT::Segment::Pt & getLastPoint(){
            return *(segment.last);
        };
        void traverseCurve(_Time_Ty x){
            _computeYValWithX(x,segment);
        };
        void setSegmentAsLine(Core::SharedPtr<typename PARENT::LinearSegmentDescriptor> & desc);
        void setSegmentAsQuadraticBezier(Core::SharedPtr<typename PARENT::QuadraticBezierSegmentDescriptor> & desc);
        void setSegmentAsCubicBezier(Core::SharedPtr<typename PARENT::CubicBezierSegmentDescriptor> & desc);
        LayerTransition_Impl(Layer *layer,_Val_Ty * val):LayerAnimation_Base<_Val_Ty,_Time_Ty>(layer,val){};
    };

    typedef LayerAnimation_Base<float,float> LayerAnimationTy;
    typedef LayerAnimation_Impl<float,float>  LayerAnimation;
    typedef LayerTransition_Impl<float,float> LayerTransition;
    
    /**
     The standard class for managing LayerAnimation real time playback.
    */
    class OMEGAWTK_EXPORT  LayerAnimationController {
        Core::SharedPtr<LayerAnimationTy> anim = nullptr;
        friend class ViewRenderTargetFrameScheduler;
        friend class ::OmegaWTK::ViewAnimator;
        bool condition;
        SharedHandle<Core::CPUThread> thread;
    protected:
        float currentFPS;
//        ViewRenderTargetFrameScheduler *scheduler = nullptr;
    public:
        virtual void setAnim(SharedHandle<LayerAnimationTy> & anim);
        void setFrameRate(float fps);
        virtual void playForward();
        virtual void pause();
        virtual void playReverse();
        LayerAnimationController();
    };

    /**
     Similar to the LayerAnimationController however it can control more than one animation simultaneously.
     @see LayerAnimationController
    */
    class OMEGAWTK_EXPORT  LayerAnimationGroupController : public LayerAnimationController {
        OmegaCommon::Vector<Core::SharedPtr<LayerAnimationTy>> anims;
    public:
        /**
         @unimplemented
         */
        void setAnim(SharedHandle<LayerAnimationTy> & anim){
            std::cout << "NULL" << std::endl;
        };
        void playForward();
        void pause();
        void playReverse();
        void addAnimation(SharedHandle<LayerAnimationTy> & anim);
        void removeAnimation(SharedHandle<LayerAnimationTy> & anim);
        LayerAnimationGroupController();
    };
};

#endif
