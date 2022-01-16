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

    /// @brief Traverse any 2D scalar.
    class OMEGAWTK_EXPORT ScalarTraverse {
        OmegaGTE::GPoint2D start_pt;
        OmegaGTE::GPoint2D end_pt;
        OmegaGTE::GPoint2D cur;
        float delta_x;
        float delta_y;
        unsigned speed;
    public:
        /// @brief Start a Traversal along the provided scalar.
        /// @param start Start Point of Scalar.
        /// @param end End Point of Scalar.
        /// @param speed The Speed of Traversal. (In units per step.)
        explicit ScalarTraverse(OmegaGTE::GPoint2D start,OmegaGTE::GPoint2D end,unsigned speed = 1);

        /// @brief Retrieve the current position of the traversal along the scalar.
        /// @returns A 2D Point.
        OmegaGTE::GPoint2D get();

        /// @brief Step forward in the curve by `speed` number of units.
        void forward();

        /// @brief Step backward in the curve by `speed` number of units.
        void back();

        /// @brief Check if current position in traversal is at start point.
        bool begin() const;

        /// @brief Check if current position in traversal is at end point.
        bool end() const;

        /// @brief Modify the scalar being traversed.
        /// @note Will only change scalar if current position intersects new scalar.
        /// @param start The New Start Point.
        /// @param end The New End Point.
        void changeScalar(OmegaGTE::GPoint2D start,OmegaGTE::GPoint2D end);
    };

    /// @brief Represents a generic mathematical linear or bezier curve used for animation.
    /// It is defined in 1 x 1 float-based coordinate space allowing it to be transposed to any 2D coordinate space.
    /// @paragraph There are three types of curves.
    /// \n - Linear -> A double point curve with a start and end point. (0,0 --- 1,1)
    /// \n - Quadratic -> A bezier curve with one control point in addition to a start and end.
    /// \n - Cubic -> A bezier with two additional control points.
    struct OMEGAWTK_EXPORT AnimationCurve {
        enum class Type : int {
            Linear,
            CubicBezier,
            QuadraticBezier
        } type;

        float start_h;
        float end_h;

        OmegaGTE::GPoint2D a = {0,0},b = {0,0};
    public:

        /// @brief Traversal of an AnimationCurve in a scaled integral 2D coordinate space.
        class Traversal {
            AnimationCurve & curve;
            void *data;

        public:
            OmegaGTE::GPoint2D get();
            void next();
            bool end();
            void reset();
            explicit Traversal(AnimationCurve & curve,float & space_w,float & space_h);
            ~Traversal();
        };
        /// @brief Create a TraversalContext using this AnimationCurve.
        /// @param st The Start Point.
        /// @param end The End Point.
        Traversal traverse(float space_w,float space_h);

        /// @brief Create a Linear AnimationCurve.
        /// @returns AnimationCurve
        static SharedHandle<AnimationCurve> Linear(float start_h,float end_h);

        /// @brief Create a Quadratic Bezier AnimationCurve.
        /// @param a The 'A' control point used in the curve.
        /// @returns AnimationCurve
        static SharedHandle<AnimationCurve> Quadratic(OmegaGTE::GPoint2D a);

        /// @brief Create a Cubic Bezier AnimationCurve.
        /// @param a The 'A' control point used in the curve.
        /// @param b The 'B' control point used in the curve.
        /// @returns AnimationCurve
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
                              const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear(0.f,1.f));
        void applyShadow(const LayerEffect::DropShadowParams & params);
        void applyTransformation(const LayerEffect::TransformationParams & params);
        void shadowTransition(const LayerEffect::DropShadowParams & from,
                              const LayerEffect::DropShadowParams &to,
                              unsigned duration,
                              const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear(0.f,1.f));
        void transformationTransition(const LayerEffect::TransformationParams & from,
                                      const LayerEffect::TransformationParams &to,
                                      unsigned duration,
                                      const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear(0.f,1.f));
        void transition(SharedHandle<CanvasFrame> & from,
                        SharedHandle<CanvasFrame> & to,
                        unsigned duration,
                        const SharedHandle<AnimationCurve> & curve = AnimationCurve::Linear(0.f,1.f));
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
