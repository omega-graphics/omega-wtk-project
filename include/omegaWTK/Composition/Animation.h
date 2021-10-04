#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/MultiThreading.h"

#include "omegaWTK/Native/NativeItem.h"

#include "Canvas.h"


#ifndef OMEGAWTK_COMPOSITION_ANIMATION_H
#define  OMEGAWTK_COMPOSITION_ANIMATION_H

namespace OmegaWTK {
    class View;
}
namespace OmegaWTK::Composition {
    class CompositorClient;
//    class ViewRenderTargetFrameScheduler;

    /**
     @interface Animation_Base
     @brief The standard class template for defining a curve-based animation
     for any value (associated with a certain composition layer) of type `_Val_Ty` 
     over a period of time of type `_Time_Ty`
     
    */
//    template<typename Val_Ty,typename Time_Ty>
//    class OMEGAWTK_EXPORT Animation_Base {
//        friend class AnimationController;
//    public:
//        enum class SegmentType : OPT_PARAM {
//            Linear,
//            CubicBezier,
//            QuadraticBezier
//        };
//    protected:
//        Val_Ty * val;
//        struct Segment {
//            SegmentType type;
//            struct Pt {
//                Time_Ty x;
//                Val_Ty y;
//            };
//            Core::UniquePtr<Pt> start;
//            Core::UniquePtr<Pt> end;
//
//            Core::Optional<Core::UniquePtr<Pt>> controlPt;
//            /**
//            Control Points used for CubicBezier Segments
//            */
//            Core::Optional<Core::UniquePtr<Pt>> controlPtA;
//            Core::Optional<Core::UniquePtr<Pt>> controlPtB;
//        };
//        void _computeYValWithX(Time_Ty & x,Segment & seg){
//            /// X can only be between 0 and 1.0 for this segment
//            switch (seg.type) {
//                case SegmentType::Linear : {
//                    auto & Pt_A = *(seg.start);
//                    auto & Pt_B = *(seg.end);
//
//                    auto m = (Pt_B.y - Pt_A.y)/(Pt_B.x - Pt_A.x);
//                    Val_Ty new_val = x * m;
//                    *(this->val) = new_val;
//
//                    break;
//                }
//                case SegmentType::CubicBezier : {
//
//                    break;
//                }
//                case SegmentType::QuadraticBezier : {
//
//                    break;
//                }
//                default:
//                    break;
//            }
//        };
//        /**
//         Modifies layer value by deltaY, based on deltaX provided.
//         */
//        virtual typename Segment::Pt & getFirstPoint() = 0;
//        virtual typename Segment::Pt & getLastPoint() = 0;
//         virtual void traverseCurve(Time_Ty x) = 0;
//    public:
//        /// Segment Descriptors
//
//        struct SegmentDescriptor {
//            Time_Ty deltaX;
//            Val_Ty deltaY;
//        };
//        typedef SegmentDescriptor LinearSegmentDescriptor;
//        struct CubicBezierSegmentDescriptor : public SegmentDescriptor {
//            /// Coordinates of the Control Points from the segment`s start point.
//            Time_Ty _delta_control_pt_a_x;
//            Val_Ty _delta_control_pt_a_y;
//            Time_Ty _delta_control_pt_b_x;
//            Val_Ty _delta_control_pt_b_y;
//        };
//        struct QuadraticBezierSegmentDescriptor : public SegmentDescriptor {
//            /// Coordinates of the Control Point from the segment`s start point.
//            Time_Ty _delta_control_pt_x;
//            Val_Ty _delta_control_pt_y;
//        };
//        template<class _Ty,std::enable_if_t<std::is_base_of_v<::OmegaWTK::Composition::Animation_Base<Val_Ty,Time_Ty>::SegmentDescriptor,_Ty>,int> = 0>
//        Core::SharedPtr<_Ty> makeDescriptor(){
//            return std::make_shared<_Ty>();
//        };
//        explicit Animation_Base(Val_Ty * val):val(val){};
//    };
//
//    template<typename Val_Ty,typename Time_Ty>
//    class OMEGAWTK_EXPORT  Animation_Impl : public Animation_Base<Val_Ty,Time_Ty> {
//        typedef Animation_Base<Val_Ty,Time_Ty> PARENT;
//        OmegaCommon::Vector<typename PARENT::Segment> segments;
//    public:
//        typename PARENT::Segment::Pt & getFirstPoint(){
//            return *(segments.front().start);
//        };
//        typename PARENT::Segment::Pt & getLastPoint(){
//            return *(segments.back().end);
//        };
//        void traverseCurve(Time_Ty x){
//
//        };
//        void addLinearSegment(Core::SharedPtr<typename PARENT::LinearSegmentDescriptor> & desc);
//        void addQuadraticBezierSegment(Core::SharedPtr<typename PARENT::QuadraticBezierSegmentDescriptor> & desc);
//        void addCubicBezierSegment(Core::SharedPtr<typename PARENT::CubicBezierSegmentDescriptor> & desc);
//        Animation_Impl(Val_Ty * val):Animation_Base<Val_Ty,Time_Ty>(val){};
//    };
//
//    template<typename Val_Ty,typename Time_Ty>
//    class OMEGAWTK_EXPORT  Transition_Impl : public Animation_Base<Val_Ty,Time_Ty> {
//        typedef Animation_Base<Val_Ty,Time_Ty> PARENT;
//        typename PARENT::Segment segment;
//    public:
//        typename PARENT::Segment::Pt & getFirstPoint(){
//            return *(segment.first);
//        };
//        typename PARENT::Segment::Pt & getLastPoint(){
//            return *(segment.last);
//        };
//        void traverseCurve(Time_Ty x){
//            _computeYValWithX(x,segment);
//        };
//        void setSegmentAsLine(Core::SharedPtr<typename PARENT::LinearSegmentDescriptor> & desc);
//        void setSegmentAsQuadraticBezier(Core::SharedPtr<typename PARENT::QuadraticBezierSegmentDescriptor> & desc);
//        void setSegmentAsCubicBezier(Core::SharedPtr<typename PARENT::CubicBezierSegmentDescriptor> & desc);
//        Transition_Impl(Val_Ty * val):Animation_Base<Val_Ty,Time_Ty>(val){};
//    };
//
//    typedef Animation_Base<float,float>  AnimationBase;
//    typedef Animation_Impl<float,float>  Animation;
//    typedef Transition_Impl<float,float> Transition;
//
//    /**
//     The standard class for managing Animation real time playback.
//    */
//    class OMEGAWTK_EXPORT  AnimationController {
//        Core::SharedPtr<AnimationBase> anim = nullptr;
//        friend class ViewRenderTargetFrameScheduler;
//        bool condition;
//
//        void *targetObject;
//    protected:
//        float currentFPS;
////        ViewRenderTargetFrameScheduler *scheduler = nullptr;
//    public:
//        virtual void setAnim(SharedHandle<AnimationBase> & anim);
//        void setFrameRate(float fps);
//        virtual void playForward();
//        virtual void pause();
//        virtual void playReverse();
//        AnimationController();
//    };
//
//    /**
//     Similar to the AnimationController however it can control more than one animation simultaneously.
//     @see AnimationController
//    */
//    class OMEGAWTK_EXPORT  AnimationGroupController : public AnimationController {
//        OmegaCommon::Vector<Core::SharedPtr<AnimationBase>> anims;
//    public:
//        /**
//         @unimplemented
//         */
//        void setAnim(SharedHandle<AnimationBase> & anim){
//            std::cout << "NULL" << std::endl;
//        };
//        void playForward();
//        void pause();
//        void playReverse();
//        void addAnimation(SharedHandle<AnimationBase> & anim);
//        void removeAnimation(SharedHandle<AnimationBase> & anim);
//        AnimationGroupController();
//    };

//    class OMEGAWTK_EXPORT  Animator {
//        Composition::CompositorClient *compClient;
////        Core::UniquePtr<Composition::ViewRenderTargetFrameScheduler> frameScheduler;
//    public:
//        struct AnimationContext {
//            Animator *anim = nullptr;
//        public:
//            Composition::AnimationController *getControllerWithID(int id);
//        };
//        typedef void (*Action)(AnimationContext context);
//    private:
//        OmegaCommon::MapVec<unsigned,Action> triggers;
//        OmegaCommon::MapVec<int,SharedHandle<Composition::AnimationController>> animControllers;
//
//    public:
//        Animator(Composition::CompositorClient *compositorClient);
//        struct TriggerDescriptor {
//            unsigned identifier;
//            Action action;
//        };
//        void assignController(int id,SharedHandle<Composition::AnimationController> & controller);
//        void addTrigger(const TriggerDescriptor & desc);
//        void activateTrigger(unsigned identifier);
//    };

    struct AnimationCurve {
        enum class Type : int {
            Linear,
            CubicBezier,
            QuadraticBezier
        } type;

        OmegaGTE::GPoint2D st_pt = {0,0};
        OmegaGTE::GPoint2D end_pt {1.0,1.0};

        OmegaGTE::GPoint2D a = {0,0},b = {0,0};
    public:
        class Traversal {
            AnimationCurve & curve;
            OmegaGTE::GPoint2D _start;
            OmegaGTE::GPoint2D _end;

            OmegaGTE::GPoint2D cur = {0.0,0.0};
            float viewport_h;
            float viewport_w;
            float curve_h;

            struct {
                OmegaGTE::GPoint2D _a_cur;
                OmegaGTE::GPoint2D _b_cur;
            } quadraticBezierT;
        public:
            OmegaGTE::GPoint2D get();
            void next();
            bool end();
            void reset();
            explicit Traversal(AnimationCurve & curve,OmegaGTE::GPoint2D & st,OmegaGTE::GPoint2D & end,float & h);
        };
        Traversal traverse(OmegaGTE::GPoint2D st,OmegaGTE::GPoint2D end,float h);
        static SharedHandle<AnimationCurve> Linear();
        static SharedHandle<AnimationCurve> Quadratic(OmegaGTE::GPoint2D a);
        static SharedHandle<AnimationCurve> Cubic(OmegaGTE::GPoint2D a,OmegaGTE::GPoint2D b);
    };

    /// @brief A generic keyframe-based animation timeline for any given duration of time.
    class OMEGAWTK_EXPORT AnimationTimeline {
    public:
        struct Keyframe;
    private:
        OmegaCommon::Vector<Keyframe> keyframes;
    public:
        /// @brief A Keyframe in a timeline.
        /// @paragraph The `time` field can ONLY be between 0 - 1.0. (0 to 100%).
        struct Keyframe {
            float time;
            SharedHandle<AnimationCurve> curve;
        };

        static SharedHandle<AnimationTimeline> Create(const OmegaCommon::Vector<Keyframe> & keyframes);
    };

    class ViewAnimator;

    class OMEGAWTK_EXPORT LayerAnimator {
        Layer & targetLayer;
        explicit LayerAnimator(Layer & layer,ViewAnimator & anim);

        ViewAnimator & parentAnimator;
    public:
        void animate(SharedHandle<CanvasFrame> & start,const SharedHandle<AnimationTimeline> & timeline,unsigned duration);
        void stop();
        void resizeTransition(unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,unsigned duration,
                              const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        void transition(SharedHandle<CanvasFrame> & from,
                        SharedHandle<CanvasFrame> & to,
                        unsigned duration,
                        const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        ~LayerAnimator() = default;
    };

    class OMEGAWTK_EXPORT ViewAnimator {
        OmegaCommon::Vector<SharedHandle<LayerAnimator>> layerAnims;
        CompositorClient *client;

        Native::NativeItemPtr nativeView;
        friend class ::OmegaWTK::View;
        friend class LayerAnimator;
        unsigned framePerSec;

        unsigned calculateTotalFrames(unsigned & duration);

    public:
        explicit ViewAnimator(CompositorClient *_client);
        void setFrameRate(unsigned _framePerSec);
        SharedHandle<LayerAnimator> layerAnimator(Layer &layer);
        void resizeTransition(unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,unsigned duration,
                    const SharedHandle<AnimationCurve> & curve);
        ~ViewAnimator() = default;
    };
};

#endif
