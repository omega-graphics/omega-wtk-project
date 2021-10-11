#include "omegaWTK/Native/NativeItem.h"

#include <queue>

#ifndef OMEGAWTK_COMPOSITION_COMPOSITORCLIENT_H
#define OMEGAWTK_COMPOSITION_COMPOSITORCLIENT_H

namespace OmegaWTK::Composition {
    class ViewRenderTarget;
    class Compositor;
    class Layer;
    struct CanvasFrame;

    typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

    class OMEGAWTK_EXPORT CompositionRenderTarget {};

    enum class CommandStatus : int {
        Ok,
        Failed,
        Delayed
    };

    class CompositorClient;

    struct CompositorCommand {
        unsigned id;
        CompositorClient & client;
        typedef enum : int {
            /// A frame draw commmand
            Render,
            /// A view command
            View,
            // A Layer resize
            LayerResize,
            /// Cancel execution of commands from client.
            Cancel,
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
        OmegaCommon::Promise<CommandStatus> status;
    };

    struct CompositorLayerResizeCommand : public CompositorCommand {
        Layer *layer;
        unsigned delta_x = 0,delta_y = 0,delta_w = 0,delta_h = 0;
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

   struct CompositorCancelCommand : public CompositorCommand {
       unsigned startID = 0,endID = 0;
   };



    /** @brief Compositor Client Proxy class for interaction with a Compositor
        @paragraph Interaction includes submitting render commands to a Compositor,
        and verifying successful frame completion. 
    */
    class OMEGAWTK_EXPORT CompositorClientProxy {
        friend class CompositorClient;

        Compositor *frontend = nullptr;

        bool recording = false;

        CompositionRenderTarget *renderTarget;

        std::queue<SharedHandle<CompositorCommand>> commandQueue;

        OmegaCommon::Async<CommandStatus> queueTimedFrame(unsigned & id,CompositorClient &client,
                                                          SharedHandle<CanvasFrame> & frame,
                                                          Timestamp & start,
                                                          Timestamp & deadline);

        OmegaCommon::Async<CommandStatus> queueFrame(unsigned & id,CompositorClient &client,
                                                     SharedHandle<CanvasFrame> & frame,
                                                     Timestamp & start);

        OmegaCommon::Async<CommandStatus> queueLayerResizeCommand(unsigned & id,CompositorClient &client,
                                                                  Layer *target,
                                                                  unsigned delta_x,
                                                                  unsigned delta_y,
                                                                  unsigned delta_w,
                                                                  unsigned delta_h,
                                                                  Timestamp &start,
                                                                  Timestamp & deadline);

        OmegaCommon::Async<CommandStatus> queueViewResizeCommand(unsigned & id,CompositorClient &client,
                                                                 Native::NativeItemPtr nativeView,
                                                                 unsigned delta_x,
                                                                 unsigned delta_y,
                                                                 unsigned delta_w,
                                                                 unsigned delta_h,
                                                                 Timestamp &start,
                                                                 Timestamp & deadline);

        OmegaCommon::Async<CommandStatus> queueCancelCommand(unsigned & id,CompositorClient &client,unsigned startID,unsigned endID);

    protected:
        void submit();
    public:
        void setFrontendPtr(Compositor *frontend);
        void beginRecord();
        void endRecord();
        virtual ~CompositorClientProxy() = default;
    };


    class OMEGAWTK_EXPORT CompositorClient {
        CompositorClientProxy & parentProxy;

        OmegaCommon::Vector<std::pair<unsigned,OmegaCommon::Async<CommandStatus>>> currentJobStatuses;

        unsigned currentCommandID = 0;

    protected:
        void pushTimedFrame(SharedHandle<CanvasFrame> & frame,Timestamp & start,Timestamp & deadline);
        void pushFrame(SharedHandle<CanvasFrame> & frame,Timestamp & start);
        void pushLayerResizeCommand(Layer *target,unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,Timestamp &start,Timestamp & deadline);
        void pushViewResizeCommand(Native::NativeItemPtr nativeView,unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,Timestamp &start,Timestamp & deadline);
        void cancelCurrentJobs();
    public:
        bool busy();

        explicit CompositorClient(CompositorClientProxy & proxy);
        virtual ~CompositorClient() = default;
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
