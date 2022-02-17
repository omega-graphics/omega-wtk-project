#include "omegaWTK/Composition/Animation.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Canvas.h"
#include <chrono>
#include <memory>
#include <cassert>

namespace OmegaWTK::Composition {

ScalarTraverse::ScalarTraverse(OmegaGTE::GPoint2D start, OmegaGTE::GPoint2D end, unsigned int speed) :
start_pt(start),
end_pt(end),
cur(start_pt),speed(speed)
{
    auto slope = (end_pt.y - start_pt.y)/(end_pt.x - start_pt.x);
    auto alpha = std::atan(slope);
    delta_x = std::cos(alpha) * float(speed);
    delta_y = slope * delta_x;

}

OmegaGTE::GPoint2D ScalarTraverse::get() {
    return cur;
}

bool ScalarTraverse::begin() const {
    return (cur.x == start_pt.x) && (cur.y == start_pt.y);
}

bool ScalarTraverse::end() const {
    return (cur.x == end_pt.x) && (cur.y == end_pt.y);
}

void ScalarTraverse::forward() {
    assert(!end() && "Reached the end of the scalar");
    cur.x += delta_x;
    cur.y += delta_y;
}

void ScalarTraverse::back() {
    assert(!begin() && "Reached the beginning of the scalar");
    cur.x -= delta_x;
    cur.y -= delta_y;
}

void ScalarTraverse::changeScalar(OmegaGTE::GPoint2D start, OmegaGTE::GPoint2D end) {
    start_pt = start;
    end_pt = end;

    auto slope = (end_pt.y - start_pt.y)/(end_pt.x - start_pt.x);

    auto alpha = std::atan(slope);
    delta_x = std::cos(alpha) * float(speed);
    delta_y = slope * delta_x;
}



SharedHandle<AnimationCurve> AnimationCurve::Linear(float start_h,float end_h) {
    auto curve = SharedHandle<AnimationCurve>(new AnimationCurve{Type::Linear,start_h,end_h});
    return curve;
}

struct AnimationCurveLinearTraversal{
    ScalarTraverse traversal;
};
struct AnimationCurveQuadraticTraversal {

    ScalarTraverse start_to_A;
    ScalarTraverse A_to_end;

    ScalarTraverse intermed;
} ;
struct AnimationCurveCubicTraversal {
    ScalarTraverse start_to_A;
    ScalarTraverse A_to_B;
    ScalarTraverse B_to_end;

    ScalarTraverse intermed_0;
    ScalarTraverse intermed_1;
    ScalarTraverse intermed_final;
};


AnimationCurve::Traversal::Traversal(AnimationCurve &curve,
                                     float & space_w,float & space_h):
                                     curve(curve),data(nullptr)
                                    {
    
    if(curve.type == Type::Linear){
        data = new AnimationCurveLinearTraversal {
            ScalarTraverse( 
            OmegaGTE::GPoint2D {0.f,curve.start_h * space_h},
            OmegaGTE::GPoint2D {space_w,curve.end_h * space_h}
            )
        };
    }
    else if(curve.type == Type::QuadraticBezier){
        data = new AnimationCurveQuadraticTraversal{
            ScalarTraverse(
                OmegaGTE::GPoint2D{0.f,curve.start_h * space_h},
                OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h}
            ),
            ScalarTraverse(
                OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h},
                OmegaGTE::GPoint2D{space_w,curve.end_h * space_h}
            ),


            ScalarTraverse(
                OmegaGTE::GPoint2D{0.f,curve.start_h * space_h},
                OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h}
            )
        };
    }
    else {
        data = new AnimationCurveCubicTraversal{
                    ScalarTraverse(
                        OmegaGTE::GPoint2D{0.f,curve.start_h * space_h},
                        OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h}
                    ),
                    ScalarTraverse(
                        OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h},
                        OmegaGTE::GPoint2D{curve.b.x * space_w,curve.b.y * space_h}
                    ),
                    ScalarTraverse(
                        OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h},
                        OmegaGTE::GPoint2D{space_w,curve.end_h * space_h}
                    ),


                    ScalarTraverse(
                        OmegaGTE::GPoint2D{0.f,curve.start_h * space_h},
                        OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h}
                    ),
                    ScalarTraverse(
                        OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h},
                        OmegaGTE::GPoint2D{curve.b.x * space_w,curve.b.y * space_h}
                    ),

                    ScalarTraverse(
                        OmegaGTE::GPoint2D{0.f,curve.start_h * space_h},
                        OmegaGTE::GPoint2D{curve.a.x * space_w,curve.a.y * space_h}
                    ),
        };
    }
}

void AnimationCurve::Traversal::next() {
    if(curve.type == Type::Linear){
       auto d = (AnimationCurveLinearTraversal *)data;
       d->traversal.forward();
    }
    else if(curve.type == Type::QuadraticBezier){
        auto d = (AnimationCurveQuadraticTraversal *)data;
        d->start_to_A.forward();
        d->A_to_end.forward();
        d->intermed.changeScalar(d->start_to_A.get(),d->A_to_end.get());
        d->intermed.forward();
    }
    else {
        auto d = (AnimationCurveCubicTraversal *)data;
        d->start_to_A.forward();
        d->A_to_B.forward();
        d->B_to_end.forward();

        d->intermed_0.changeScalar(d->start_to_A.get(),d->A_to_B.get());
        d->intermed_1.changeScalar(d->A_to_B.get(),d->B_to_end.get());
        d->intermed_0.forward();
        d->intermed_1.forward();

        d->intermed_final.changeScalar(d->intermed_0.get(),d->intermed_1.get());
        d->intermed_final.forward();
    }
}

OmegaGTE::GPoint2D AnimationCurve::Traversal::get() {
    if(curve.type == Type::Linear){
       auto d = (AnimationCurveLinearTraversal *)data;
       return d->traversal.get();
    }
    else if(curve.type == Type::QuadraticBezier){
        auto d = (AnimationCurveQuadraticTraversal *)data;
        return d->intermed.get();
    }
    else {
        auto d = (AnimationCurveCubicTraversal *)data;
        return d->intermed_final.get();
    }
}

bool AnimationCurve::Traversal::end() {
    if(curve.type == Type::Linear){
       auto d = (AnimationCurveLinearTraversal *)data;
       return d->traversal.end();
    }
    else if(curve.type == Type::QuadraticBezier){
        auto d = (AnimationCurveQuadraticTraversal *)data;
        return d->intermed.end();
    }
    else {
        auto d = (AnimationCurveCubicTraversal *)data;
        return d->intermed_final.end();
    }
}

void AnimationCurve::Traversal::reset() {
   
}

AnimationCurve::Traversal::~Traversal(){
    if(curve.type == Type::Linear){
        delete (AnimationCurveLinearTraversal *)data;
    }
    else if(curve.type == Type::QuadraticBezier){
        delete (AnimationCurveQuadraticTraversal *)data;
    }
    else {
        delete (AnimationCurveCubicTraversal *)data;
    }
}

AnimationCurve::Traversal AnimationCurve::traverse(float space_w,float space_h) {
    return Traversal(*this,space_w,space_h);
}

AnimationTimeline::Keyframe AnimationTimeline::Keyframe::CanvasFrameStop(float time, SharedHandle<AnimationCurve> curve,
                                                          SharedHandle<CanvasFrame> &frame) {
    Keyframe k {};
    k.time = time;
    k.curve = std::move(curve);
    k.frame = frame;
    k.effect = nullptr;
    return k;
}

AnimationTimeline::Keyframe AnimationTimeline::Keyframe::DropShadowStop(float time, SharedHandle<AnimationCurve> curve,
                                                                        LayerEffect::DropShadowParams &params) {
    Keyframe k {};
    k.time = time;
    k.curve = std::move(curve);
    k.frame = nullptr;

    k.effect = std::make_shared<LayerEffect>(LayerEffect{LayerEffect::DropShadow,nullptr});
    k.effect->params = new LayerEffect::DropShadowParams(params);

    return k;
}

AnimationTimeline::Keyframe AnimationTimeline::Keyframe::TransformationStop(float time,
                                                                            SharedHandle<AnimationCurve> curve,
                                                                            LayerEffect::TransformationParams &params) {
    Keyframe k {};
    k.time = time;
    k.curve = std::move(curve);
    k.frame = nullptr;

    k.effect = std::make_shared<LayerEffect>(LayerEffect{LayerEffect::Transformation,nullptr});
    k.effect->params = new LayerEffect::TransformationParams(params);

    return k;
}


SharedHandle<AnimationTimeline> AnimationTimeline::Create(const OmegaCommon::Vector<Keyframe> &keyframes) {
    auto object = std::make_shared<AnimationTimeline>();
    bool canvasFrameStop = (bool)keyframes.front().frame;

    if(!canvasFrameStop) {
        bool shadowFrameStop = keyframes.front().effect->type == LayerEffect::DropShadow;

        for(auto & k : keyframes){
            if(shadowFrameStop){
                assert(k.effect->type == LayerEffect::DropShadow && "All keyframes must animate the Drop Shadow effect Only");
            }
            else {
                assert(k.effect->type == LayerEffect::Transformation && "All keyframes must animate the Transformation effect Only");
            }
        }
    }
    else {
        for(auto & k : keyframes){
            assert(k.frame && "All keyframes must be a typeof Canvas Frame Stop.");
        }
    }
    object->keyframes = keyframes;
    return object;
}



LayerAnimator::LayerAnimator(Layer &layer,ViewAnimator & parentAnimator):CompositorClient(parentAnimator), targetLayer(layer) ,parentAnimator(parentAnimator){
    
}


void LayerAnimator::transition(SharedHandle<CanvasFrame> &from, SharedHandle<CanvasFrame> &to, unsigned duration,
                                   const SharedHandle<AnimationCurve> &curve) {
    assert(duration > 0 && "Cannot have null duration");
    auto totalFrames = parentAnimator.calculateTotalFrames(duration);


}

void LayerAnimator::resizeTransition(unsigned int delta_x, unsigned int delta_y, unsigned int delta_w,
                                         unsigned int delta_h, unsigned duration,
                                         const SharedHandle<AnimationCurve> &curve) {
    auto totalFrames = parentAnimator.calculateTotalFrames(duration);
    Timestamp timestamp = std::chrono::high_resolution_clock::now();
    auto frameInterval = std::chrono::milliseconds(duration/totalFrames);
    Timestamp deadline = timestamp + frameInterval;
    for(;totalFrames > 0;totalFrames--){
        pushLayerResizeCommand(&targetLayer,
                                                        delta_x/totalFrames,
                                                        delta_y/totalFrames,
                                                        delta_w/totalFrames,
                                                        delta_h/totalFrames,timestamp,deadline);
        deadline += frameInterval;
    }
}

void LayerAnimator::animate(const SharedHandle<AnimationTimeline> &timeline,
                                unsigned duration) {
    assert(duration > 0 && "Cannot have null duration");

}

 void LayerAnimator::pause() {
     cancelCurrentJobs();
 }

unsigned int ViewAnimator::calculateTotalFrames(unsigned int &duration) {
    assert(duration > 0 && "Cannot have null duration");
    auto framesPerSec = (float)framePerSec;
    auto totalFrames = ((float)duration/1000.f) * framesPerSec;
    float extra;
    std::modf(totalFrames,&extra);
    assert((extra == 0.f) && "Cannot have animation with incomplete frame intervals");
    return (unsigned) totalFrames;
}

ViewAnimator::ViewAnimator(CompositorClientProxy & _client):CompositorClient(_client), _client(_client),framePerSec(30){

}

void ViewAnimator::pause() {
    cancelCurrentJobs();
}

void ViewAnimator::setFrameRate(unsigned int _framePerSec) {
    assert(_framePerSec > 1 && "Must have more than 1 frame per second");
    framePerSec = _framePerSec;
}

void ViewAnimator::resizeTransition(unsigned int delta_x, unsigned int delta_y, unsigned int delta_w,
                                        unsigned int delta_h, unsigned int duration,
                                        const SharedHandle<AnimationCurve> &curve) {
    auto totalFrames = calculateTotalFrames(duration);
    Timestamp timestamp = std::chrono::high_resolution_clock::now();
    auto frameInterval = std::chrono::milliseconds(duration/totalFrames);
    Timestamp deadline = timestamp + frameInterval;
    for(;totalFrames > 0;totalFrames--){
        pushViewResizeCommand(nativeView,
                                                       delta_x/totalFrames,
                                                       delta_y/totalFrames,
                                                       delta_w/totalFrames,
                                                       delta_h/totalFrames,timestamp,deadline);
        deadline += frameInterval;
    }
}


};
