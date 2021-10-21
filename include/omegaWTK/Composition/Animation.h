#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Core/MultiThreading.h"

#include "omegaWTK/Native/NativeItem.h"


#include "Layer.h"
#include "Canvas.h"

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

            SharedHandle<CanvasFrame> frame;
            SharedHandle<LayerEffect> effect;

            static Keyframe CanvasFrameStop(float time,SharedHandle<AnimationCurve> curve,SharedHandle<CanvasFrame> & frame);
            static Keyframe DropShadowStop(float time,SharedHandle<AnimationCurve> curve,LayerEffect::DropShadowParams & params);
            static Keyframe TransformationStop(float time,SharedHandle<AnimationCurve> curve,LayerEffect::TransformationParams & params);
        };

        static SharedHandle<AnimationTimeline> Create(const OmegaCommon::Vector<Keyframe> & keyframes);
    };

    class ViewAnimator;

    class OMEGAWTK_EXPORT LayerAnimator : public CompositorClient {
        Layer & targetLayer;
        ViewAnimator &parentAnimator;
        explicit LayerAnimator(Layer & layer,ViewAnimator &parentAnimator);
    public:
        void setFrameRate(unsigned _framePerSec);
        void animate(const SharedHandle<AnimationTimeline> & timeline,unsigned duration);
        void pause();
        void resume();
        void resizeTransition(unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,unsigned duration,
                              const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        void applyShadow(const LayerEffect::DropShadowParams & params);
        void applyTransformation(const LayerEffect::TransformationParams & params);
        void shadowTransition(const LayerEffect::DropShadowParams & from,
                              const LayerEffect::DropShadowParams &to,
                              unsigned duration,
                              const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        void transformationTransition(const LayerEffect::TransformationParams & from,
                                      const LayerEffect::TransformationParams &to,
                                      unsigned duration,
                                      const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        void transition(SharedHandle<CanvasFrame> & from,
                        SharedHandle<CanvasFrame> & to,
                        unsigned duration,
                        const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear());
        ~LayerAnimator() = default;
    };

    class OMEGAWTK_EXPORT ViewAnimator : public CompositorClient {
        OmegaCommon::Vector<SharedHandle<LayerAnimator>> layerAnims;

        CompositorClientProxy & _client;

        Native::NativeItemPtr nativeView;
        friend class ::OmegaWTK::View;
        friend class LayerAnimator;
        unsigned framePerSec;

        unsigned calculateTotalFrames(unsigned & duration);

    public:
        explicit ViewAnimator(CompositorClientProxy & _client);
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
