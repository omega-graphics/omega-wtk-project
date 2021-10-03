#include "omegaWTK/Native/NativeItem.h"
#include "omegaWTK/Composition/Canvas.h"

#include <queue>

#ifndef OMEGAWTK_COMPOSITION_COMPOSITORCLIENT_H
#define OMEGAWTK_COMPOSITION_COMPOSITORCLIENT_H

namespace OmegaWTK::Composition {
    class ViewRenderTarget;
    class Compositor;

    typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

    class OMEGAWTK_EXPORT CompositionRenderTarget {

    };

    struct CompositorCommand {
        typedef enum : int {
            Render,
            View,
            LayerResize,
            HoldRender,
            ResumeRender
        } Type;
        Type type;
        typedef enum {
            Low,
            High
        } Priority;
        Priority priority;
        struct {
            bool hasThreshold = false;
            Timestamp timeStamp;
            Timestamp threshold;
        }thresholdParams;
    };

    struct CompositorLayerResizeCommand : public CompositorCommand {
        Layer *layer;
        unsigned delta_x = 0,delta_y = 0,delta_w = 0,delta_h = 0;
    };

    struct CompositorHoldRenderCommand : public CompositorCommand {
        Layer *layer;
    };

    struct CompositorResumeRenderCommand : public CompositorCommand {
        Layer *layer;
    };

   struct CompositionRenderCommand : public CompositorCommand {
       CompositionRenderTarget *renderTarget;
       SharedHandle<CanvasFrame> frame;
   };

   struct CompositorViewCommand : public CompositorCommand {
       typedef enum : int {
           Resize
       } CommandType;
       CommandType subType;
       Native::NativeItemPtr viewPtr;
       unsigned delta_x = 0,delta_y = 0,delta_w = 0,delta_h = 0;
   };



    /** @brief Compositor Client class for interaction with a Compositor
        @paragraph Interaction includes submitting render commands to a Compositor,
        and verifying successful frame completion. 
    */
    class OMEGAWTK_EXPORT CompositorClient {
        friend class Canvas;
        friend class LayerAnimator;
        friend class ViewAnimator;

        Compositor *frontend = nullptr;

        CompositionRenderTarget *renderTarget;

        std::queue<SharedHandle<CompositorCommand>> commandQueue;

        void queueTimedFrame(SharedHandle<CanvasFrame> & frame,Timestamp & start,Timestamp & deadline);
        void queueFrame(SharedHandle<CanvasFrame> & frame,Timestamp & start);
        void queueLayerResizeCommand(Layer *target,unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,Timestamp &start,Timestamp & deadline);
        void queueViewResizeCommand(Native::NativeItemPtr nativeView,unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,Timestamp &start,Timestamp & deadline);
        void queueStopForRenderingLayer(Layer *target);
        void queueResumeForRenderingLayer(Layer *target);
    protected:
        void submit();

    public:
        void setFrontendPtr(Compositor *frontend);
        virtual void commitRender() = 0;
        virtual ~CompositorClient();
    };

    /** @brief A CompositorClient that only allows for video frame submission/verification.
        @paragraph Interaction includes submitting render commands to a Compositor,
        and verifying successful frame completion. 
    */
//    class OMEGAWTK_EXPORT CompositorVideoClient {
//        Compositor *frontend = nullptr;
//
//        OmegaCommon::Vector<SharedHandle<CanvasFrame>> drawQueue;
//    protected:
//        void queueFrame(Core::SharedPtr<Media::BitmapImage> & img);
//        void submit(CompositionRenderTarget *renderTarget);
//    public:
//        void setFrontendPtr(Compositor *frontend);
//        virtual void commitCurrentFrame() = 0;
//        virtual ~CompositorVideoClient();
//    };

    

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
