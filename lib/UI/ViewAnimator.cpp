#include "omegaWTK/UI/ViewAnimator.h"
#include <chrono>

namespace OmegaWTK {

ViewAnimator::ViewAnimator(Core::UniquePtr<Composition::ViewRenderTarget> & renderTarget,
                           Composition::Compositor *compositor){
    
};

Composition::LayerAnimationController * ViewAnimator::AnimationContext::getControllerWithID(int id){
    return anim->animControllers[id].get();
};

void ViewAnimator::addTrigger(const TriggerDescriptor & desc){
    triggers.insert(std::make_pair(desc.identifier,desc.action));
};

void ViewAnimator::activateTrigger(unsigned identifier){
    AnimationContext context;
    context.anim = this;
    triggers[identifier](std::move(context));
};

void ViewAnimator::assignController(int id,SharedHandle<Composition::LayerAnimationController> & controller){
    animControllers.insert(std::make_pair(id,controller));
};

void ViewAnimator::scheduleFrame(std::chrono::milliseconds deadline_delta){
    auto current_time_stamp = std::chrono::high_resolution_clock::now();
    auto deadline_timp_stamp = current_time_stamp + deadline_delta;
};

//SharedHandle<ViewAnimator> ViewAnimator::Create(View *view){
//    return SharedHandle<ViewAnimator>(new ViewAnimator(view->renderTarget,view->getWidgetCompositor()));
//};

}
