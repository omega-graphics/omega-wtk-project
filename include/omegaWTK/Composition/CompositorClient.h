#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Composition/Canvas.h"

#ifndef OMEGAWTK_COMPOSITION_COMPOSITORCLIENT_H
#define OMEGAWTK_COMPOSITION_COMPOSITORCLIENT_H

namespace OmegaWTK::Composition {
    class ViewRenderTarget;
    class Compositor;

//    class ViewRenderTargetFrameScheduler {
//        Compositor *compositor;
//    protected:
////        ViewRenderTargetFrameScheduler(Compositor *compositor);
//        bool canScheduleFrameRegardingCompositorCommandBufferStatus(float timeDelta);
//        virtual bool canScheduleFrameRegardingNativeItemStatus(float timeDelta);
//    public:
//        /**
//         Can we schedule a redraw at the time delta from now.
//         */
//        bool canScheduleFrame(float timeDelta);
//        /**
//         Schedule a frame creation/redraw at the time delta from now.
//         */
//        void scheduleFrame(float timeDelta);
//        static Core::UniquePtr<ViewRenderTargetFrameScheduler> Create(Core::UniquePtr<ViewRenderTarget> & renderTarget,Compositor *compositor);
//    };

    class OMEGAWTK_EXPORT CompositionRenderTarget {
        
    };

    /** @brief Compositor Client class for interaction with a Compositor
        @paragraph Interaction includes submitting render commands to a Compositor,
        and verifying successful frame completion. 
    */
    
    class OMEGAWTK_EXPORT CompositorClient {
        friend class Canvas;

        Compositor *frontend;

        Core::Vector<VisualCommand> drawQueue;

        void queueVisualCommand(VisualCommand *v);
        void submit(CompositionRenderTarget *renderTarget);
    };

    

    /**
     The Compositor's interface for composing to a widget's view.
     */
    class OMEGAWTK_EXPORT ViewRenderTarget : public CompositionRenderTarget {
        Native::NativeItemPtr native;
    public:
        Native::NativeItemPtr getNativePtr();
        ViewRenderTarget(Native::NativeItemPtr _native);
        ~ViewRenderTarget();
    };

    
};


#endif
