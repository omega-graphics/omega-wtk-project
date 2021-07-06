#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/MultiThreading.h"


#ifndef OMEGAWTK_COMPOSITION_ANIMATION_H
#define  OMEGAWTK_COMPOSITION_ANIMATION_H


namespace OmegaWTK::Composition {
    class CompositorClient;
//    class ViewRenderTargetFrameScheduler;

    /**
     @interface Animation_Base
     @brief The standard class template for defining a curve-based animation
     for any value (associated with a certain composition layer) of type `_Val_Ty` 
     over a period of time of type `_Time_Ty`
     
    */
    template<typename _Val_Ty,typename _Time_Ty>
    class OMEGAWTK_EXPORT Animation_Base {
        friend class AnimationController;
    public:
        enum class SegmentType : OPT_PARAM {
            Linear,
            CubicBezier,
            QuadraticBezier
        };
    protected:
        _Val_Ty * val;
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
            /// X can only be between 0 and 1.0 for this segment
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
        template<class _Ty,std::enable_if_t<std::is_base_of_v<::OmegaWTK::Composition::Animation_Base<_Val_Ty,_Time_Ty>::SegmentDescriptor,_Ty>,int> = 0>
        Core::SharedPtr<_Ty> makeDescriptor(){
            return std::make_shared<_Ty>();
        };
        Animation_Base(_Val_Ty * val):val(val){};
    };

    template<typename _Val_Ty,typename _Time_Ty>
    class OMEGAWTK_EXPORT  Animation_Impl : public Animation_Base<_Val_Ty,_Time_Ty> {
        typedef Animation_Base<_Val_Ty,_Time_Ty> PARENT;
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
        Animation_Impl(_Val_Ty * val):Animation_Base<_Val_Ty,_Time_Ty>(val){};
    };

    template<typename _Val_Ty,typename _Time_Ty>
    class OMEGAWTK_EXPORT  Transition_Impl : public Animation_Base<_Val_Ty,_Time_Ty> {
        typedef Animation_Base<_Val_Ty,_Time_Ty> PARENT;
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
        Transition_Impl(_Val_Ty * val):Animation_Base<_Val_Ty,_Time_Ty>(val){};
    };

    typedef Animation_Base<float,float>  AnimationBase;
    typedef Animation_Impl<float,float>  Animation;
    typedef Transition_Impl<float,float> Transition;
    
    /**
     The standard class for managing Animation real time playback.
    */
    class OMEGAWTK_EXPORT  AnimationController {
        Core::SharedPtr<AnimationBase> anim = nullptr;
        friend class ViewRenderTargetFrameScheduler;
        bool condition;

        void *targetObject;
    protected:
        float currentFPS;
//        ViewRenderTargetFrameScheduler *scheduler = nullptr;
    public:
        virtual void setAnim(SharedHandle<AnimationBase> & anim);
        void setFrameRate(float fps);
        virtual void playForward();
        virtual void pause();
        virtual void playReverse();
        AnimationController();
    };

    /**
     Similar to the AnimationController however it can control more than one animation simultaneously.
     @see AnimationController
    */
    class OMEGAWTK_EXPORT  AnimationGroupController : public AnimationController {
        OmegaCommon::Vector<Core::SharedPtr<AnimationBase>> anims;
    public:
        /**
         @unimplemented
         */
        void setAnim(SharedHandle<AnimationBase> & anim){
            std::cout << "NULL" << std::endl;
        };
        void playForward();
        void pause();
        void playReverse();
        void addAnimation(SharedHandle<AnimationBase> & anim);
        void removeAnimation(SharedHandle<AnimationBase> & anim);
        AnimationGroupController();
    };

    class OMEGAWTK_EXPORT  Animator {
        Composition::CompositorClient *compClient;
//        Core::UniquePtr<Composition::ViewRenderTargetFrameScheduler> frameScheduler;
    public:
        struct AnimationContext {
            Animator *anim = nullptr;
        public:
            Composition::AnimationController *getControllerWithID(int id);
        };
        typedef void (*Action)(AnimationContext context);
    private:
        OmegaCommon::MapVec<unsigned,Action> triggers;
        OmegaCommon::MapVec<int,SharedHandle<Composition::AnimationController>> animControllers;
        
    public:
        Animator(Composition::CompositorClient *compositorClient);
        struct TriggerDescriptor {
            unsigned identifier;
            Action action;
        };
        void assignController(int id,SharedHandle<Composition::AnimationController> & controller);
        void addTrigger(const TriggerDescriptor & desc);
        void activateTrigger(unsigned identifier);
    };
};

#endif
