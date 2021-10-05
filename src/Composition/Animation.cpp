#include "omegaWTK/Composition/Animation.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include <chrono>

namespace OmegaWTK::Composition {
//    void AnimationController::setAnim(SharedHandle<AnimationBase> &anim){
//        this->anim = anim;
//    };
//
//    void AnimationController::setFrameRate(float fps){
//        currentFPS = fps;
//    };
//
//    void AnimationController::playForward(){
//        // assert(!thread && "Cannot Open new thread while playing!");
//        // thread = Core::CPUThread::OpenWithVoidReturn([&](Core::CPUThread *){
//        //     while(true){
//        //         anim->traverseCurve(1.f/currentFPS);
//        //         if(condition){
//        //             break;
//        //         };
//        //     };
//        // });
//    };
//    void AnimationController::playReverse(){
//        // assert(!thread && "Cannot Open new thread while playing!");
//        // thread = Core::CPUThread::OpenWithVoidReturn([&](Core::CPUThread *){
//        //     while(true){
//        //         anim->traverseCurve(-1.f/currentFPS);
//        //         if(condition){
//        //             break;
//        //         };
//        //     };
//        // });
//    };
//    void AnimationController::pause(){
//        condition = true;
//        // thread->close();
//        // thread.reset();
//        // condition = false;
//    };
//
//    void AnimationGroupController::addAnimation(SharedHandle<AnimationBase> &anim){
//        anims.push_back(anim);
//    };
//
//    void AnimationGroupController::removeAnimation(SharedHandle<AnimationBase> &anim){
//        auto anim_it = anims.begin();
//        while(anim_it != anims.end()){
//            auto _anim = *anim_it;
//            if(_anim == anim){
//                anims.erase(anim_it);
//                return;
//                break;
//            };
//            ++anim_it;
//        };
//    };
//
//    void AnimationGroupController::playForward(){
//
//    };
//    void AnimationGroupController::playReverse(){
//
//    };
//
//    void AnimationGroupController::pause(){
//
//    };
//
//
//
//
//
//
//Animator::Animator(Composition::CompositorClient *compCient):compClient(compCient){
//
//};
//
//Composition::AnimationController * Animator::AnimationContext::getControllerWithID(int id){
//    return anim->animControllers[id].get();
//};
//
//void Animator::addTrigger(const TriggerDescriptor & desc){
//    triggers.insert(std::make_pair(desc.identifier,desc.action));
//};
//
//void Animator::activateTrigger(unsigned identifier){
//    AnimationContext context;
//    context.anim = this;
//    auto action = triggers[identifier];
//    action((std::move(context)));
//};
//
//void Animator::assignController(int id,SharedHandle<Composition::AnimationController> & controller){
//    animControllers.insert(std::make_pair(id,controller));
//};

// void Animator::scheduleFrame(std::chrono::milliseconds deadline_delta){
//     auto current_time_stamp = std::chrono::high_resolution_clock::now();
//     auto deadline_timp_stamp = current_time_stamp + deadline_delta;
// };


SharedHandle<AnimationCurve> AnimationCurve::Linear() {
    auto curve = SharedHandle<AnimationCurve>(new AnimationCurve{Type::Linear});
    return curve;
}


AnimationCurve::Traversal::Traversal(AnimationCurve &curve,
                                     OmegaGTE::GPoint2D &st,
                                     OmegaGTE::GPoint2D &end,
                                     float &h):
                                     curve(curve),
                                     _start(st),
                                     _end(end),
                                     viewport_h(h),viewport_w(end.x - st.x){
    if(curve.type == Type::Linear){
        curve_h = _end.y - _start.y;
    }
    else if(curve.type == Type::QuadraticBezier){
        quadraticBezierT._a_cur = st;
        quadraticBezierT._b_cur = curve.a;
    }
    cur = OmegaGTE::GPoint2D{_start.x/viewport_w,_start.y/viewport_h};
}

void AnimationCurve::Traversal::next() {
    if(curve.type == Type::Linear){
        cur.x += (1.f/viewport_w);
        cur.y += (1.f/curve_h);
    }
}

OmegaGTE::GPoint2D AnimationCurve::Traversal::get() {
    return OmegaGTE::GPoint2D{cur.x * viewport_w,cur.y * viewport_h};
}

bool AnimationCurve::Traversal::end() {
    return cur.x == 1.f;
}

void AnimationCurve::Traversal::reset() {
    cur = OmegaGTE::GPoint2D {0.f,0.f};
}

AnimationCurve::Traversal AnimationCurve::traverse(OmegaGTE::GPoint2D st, OmegaGTE::GPoint2D end, float h) {
    return Traversal(*this,st,end,h);
}




LayerAnimator::LayerAnimator(Layer &layer,ViewAnimator & anim):targetLayer(layer),parentAnimator(anim) {

}

void LayerAnimator::transition(SharedHandle<CanvasFrame> &from, SharedHandle<CanvasFrame> &to, unsigned duration,
                                   const SharedHandle<AnimationCurve> &curve) {
    assert(duration > 0 && "Cannot have null duration");

}

void LayerAnimator::resizeTransition(unsigned int delta_x, unsigned int delta_y, unsigned int delta_w,
                                         unsigned int delta_h, unsigned duration,
                                         const SharedHandle<AnimationCurve> &curve) {
    auto totalFrames = parentAnimator.calculateTotalFrames(duration);
    Timestamp timestamp = std::chrono::high_resolution_clock::now();
    auto frameInterval = std::chrono::milliseconds(duration/totalFrames);
    Timestamp deadline = timestamp + frameInterval;
    for(;totalFrames > 0;totalFrames--){
        parentAnimator.client->queueLayerResizeCommand(&targetLayer,
                                                        delta_x/totalFrames,
                                                        delta_y/totalFrames,
                                                        delta_w/totalFrames,
                                                        delta_h/totalFrames,timestamp,deadline);
        deadline += frameInterval;
    }
}

void LayerAnimator::animate(SharedHandle<CanvasFrame> &start, const SharedHandle<AnimationTimeline> &timeline,
                                unsigned duration) {
    assert(duration > 0 && "Cannot have null duration");

}

void LayerAnimator::stop() {
    parentAnimator.client->queueStopForRenderingLayer(&targetLayer);
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

ViewAnimator::ViewAnimator(CompositorClient *_client):client(_client),framePerSec(30){

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
        client->queueViewResizeCommand(nativeView,
                                                       delta_x/totalFrames,
                                                       delta_y/totalFrames,
                                                       delta_w/totalFrames,
                                                       delta_h/totalFrames,timestamp,deadline);
        deadline += frameInterval;
    }
}


};
