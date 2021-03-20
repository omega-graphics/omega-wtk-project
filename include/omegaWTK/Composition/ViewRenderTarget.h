#include "omegaWTK/Native/NativeItem.h"

#ifndef OMEGAWTK_COMPOSITION_VIEWRENDERTARGET_H
#define OMEGAWTK_COMPOSITION_VIEWRENDERTARGET_H

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
    /**
     The Compositor's interface for composing to a widget's view.
     */
    class OMEGAWTK_EXPORT ViewRenderTarget {
        Native::NativeItemPtr native;
    public:
        Native::NativeItemPtr getNativePtr();
        ViewRenderTarget(Native::NativeItemPtr _native);
        ~ViewRenderTarget();
    };
};


#endif
