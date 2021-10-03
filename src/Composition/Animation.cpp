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


SharedHandle<AnimationCurve> AnimationCurve::Linear(float y) {
    auto curve = SharedHandle<AnimationCurve>(new AnimationCurve{Type::Linear});
    curve->end_pt.y = y;
    return curve;
}

void AnimationCurve::reset() {
    current_pt = OmegaGTE::GPoint2D {0,0};
}

float AnimationCurve::get() {
    
}

bool AnimationCurve::end() {
    return current_pt.x == end_pt.x;
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
