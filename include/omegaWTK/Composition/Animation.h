#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/MultiThreading.h"

#include "omegaWTK/Native/NativeItem.h"


#include "Canvas.h"
#include "CompositorClient.h"

#ifndef OMEGAWTK_COMPOSITION_ANIMATION_H
#define  OMEGAWTK_COMPOSITION_ANIMATION_H

namespace OmegaWTK {
    class View;
}
namespace OmegaWTK::Composition {

    struct OMEGAWTK_EXPORT AnimationCurve {
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

    class OMEGAWTK_EXPORT LayerAnimator : public CompositorClient {
        Layer & targetLayer;
        explicit LayerAnimator(Layer & layer);
    public:
        void setFrameRate(unsigned _framePerSec);
        void animate(SharedHandle<CanvasFrame> & start,const SharedHandle<AnimationTimeline> & timeline,unsigned duration);
        void pause();
        void resume();
        void resizeTransition(unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,unsigned duration,
                              const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        void transition(SharedHandle<CanvasFrame> & from,
                        SharedHandle<CanvasFrame> & to,
                        unsigned duration,
                        const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        ~LayerAnimator() = default;
    };

    class OMEGAWTK_EXPORT ViewAnimator : public CompositorClient {
        OmegaCommon::Vector<SharedHandle<LayerAnimator>> layerAnims;

        Native::NativeItemPtr nativeView;
        friend class ::OmegaWTK::View;
        friend class LayerAnimator;
        unsigned framePerSec;

        unsigned calculateTotalFrames(unsigned & duration);

    public:
        explicit ViewAnimator(CompositorClientProxy *_client);
        void setFrameRate(unsigned _framePerSec);
        void pause();
        void resume();
        SharedHandle<LayerAnimator> layerAnimator(Layer &layer);
        void resizeTransition(unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,unsigned duration,
                    const SharedHandle<AnimationCurve> & curve);
        ~ViewAnimator() = default;
    };
};

#endif
