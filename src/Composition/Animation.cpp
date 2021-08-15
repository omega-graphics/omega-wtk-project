#include "omegaWTK/Composition/Animation.h"
#include <chrono>

namespace OmegaWTK::Composition {
    void AnimationController::setAnim(SharedHandle<AnimationBase> &anim){
        this->anim = anim;
    };

    void AnimationController::setFrameRate(float fps){
        currentFPS = fps;
    };

    void AnimationController::playForward(){
        // assert(!thread && "Cannot Open new thread while playing!");
        // thread = Core::CPUThread::OpenWithVoidReturn([&](Core::CPUThread *){
        //     while(true){
        //         anim->traverseCurve(1.f/currentFPS);
        //         if(condition){
        //             break;
        //         };
        //     };
        // });
    };
    void AnimationController::playReverse(){
        // assert(!thread && "Cannot Open new thread while playing!");
        // thread = Core::CPUThread::OpenWithVoidReturn([&](Core::CPUThread *){
        //     while(true){
        //         anim->traverseCurve(-1.f/currentFPS);
        //         if(condition){
        //             break;
        //         };
        //     };
        // });
    };
    void AnimationController::pause(){
        condition = true;
        // thread->close();
        // thread.reset();
        // condition = false;
    };

    void AnimationGroupController::addAnimation(SharedHandle<AnimationBase> &anim){
        anims.push_back(anim);
    };

    void AnimationGroupController::removeAnimation(SharedHandle<AnimationBase> &anim){
        auto anim_it = anims.begin();
        while(anim_it != anims.end()){
            auto _anim = *anim_it;
            if(_anim == anim){
                anims.erase(anim_it);
                return;
                break;
            };
            ++anim_it;
        };
    };

    void AnimationGroupController::playForward(){

    };
    void AnimationGroupController::playReverse(){
        
    };

    void AnimationGroupController::pause(){
        
    };






Animator::Animator(Composition::CompositorClient *compCient):compClient(compCient){
    
};

Composition::AnimationController * Animator::AnimationContext::getControllerWithID(int id){
    return anim->animControllers[id].get();
};

void Animator::addTrigger(const TriggerDescriptor & desc){
    triggers.insert(std::make_pair(desc.identifier,desc.action));
};

void Animator::activateTrigger(unsigned identifier){
    AnimationContext context;
    context.anim = this;
    auto action = triggers[identifier];
    action((std::move(context)));
};

void Animator::assignController(int id,SharedHandle<Composition::AnimationController> & controller){
    animControllers.insert(std::make_pair(id,controller));
};

// void Animator::scheduleFrame(std::chrono::milliseconds deadline_delta){
//     auto current_time_stamp = std::chrono::high_resolution_clock::now();
//     auto deadline_timp_stamp = current_time_stamp + deadline_delta;
// };


};
