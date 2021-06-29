#include "View.h"
#include "omegaWTK/Composition/CompositorClient.h"
#include <chrono>

#ifndef OMEGAWTK_UI_VIEWANIMATOR_H
#define OMEGAWTK_UI_VIEWANIMATOR_H
namespace OmegaWTK {
    /**
     @brief A CanvasView's Animation Execution Engine and Scheduler
     @discussion
     */
 class OMEGAWTK_EXPORT  ViewAnimator {
//        Core::UniquePtr<Composition::ViewRenderTargetFrameScheduler> frameScheduler;
        friend class Composition::LayerAnimationController;
    public:
        struct AnimationContext {
            ViewAnimator *anim = nullptr;
        public:
            Composition::LayerAnimationController *getControllerWithID(int id);
        };
        typedef void (*Action)(AnimationContext context);
    private:
        OmegaCommon::Map<unsigned,Action> triggers;
        OmegaCommon::Map<int,SharedHandle<Composition::LayerAnimationController>> animControllers;
        void scheduleFrame(std::chrono::milliseconds deadline_delta);
        ViewAnimator(Core::UniquePtr<Composition::ViewRenderTarget> & renderTarget,Composition::Compositor *compositor);
    public:
        struct TriggerDescriptor {
            unsigned identifier;
            Action action;
        };
        void assignController(int id,SharedHandle<Composition::LayerAnimationController> & controller);
        void addTrigger(const TriggerDescriptor & desc);
        void activateTrigger(unsigned identifier);
//        static SharedHandle<ViewAnimator> Create(View *view);
    };

};
#endif
