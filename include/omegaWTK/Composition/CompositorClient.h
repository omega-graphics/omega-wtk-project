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
        CompositorCommand(unsigned id,
                        CompositorClient &client,
                        Type type,
                        Priority priority,
                        decltype(thresholdParams) thresholdParams,
                        OmegaCommon::Promise<CommandStatus> status):
        id(id),
        client(client),
        type(type),
        priority(priority),
        thresholdParams(thresholdParams),
        status(std::move(status))
        {

        };
        virtual ~CompositorCommand() = default;
    };

    struct CompositorLayerResizeCommand : public CompositorCommand {
        Layer *layer;
        unsigned delta_x = 0,delta_y = 0,delta_w = 0,delta_h = 0;
        CompositorLayerResizeCommand(unsigned id,
                                    CompositorClient &client,
                                    Type type,
                                    Priority priority,
                                    decltype(CompositorCommand::thresholdParams) thresholdParams,
                                    OmegaCommon::Promise<CommandStatus> status,
                                    Layer *layer,
                                    unsigned delta_x,
                                    unsigned delta_y,
                                    unsigned delta_w,
                                    unsigned delta_h): CompositorCommand(id,client,type,priority,thresholdParams,std::move(status)),
                                    layer(layer),
                                    delta_x(delta_x),
                                    delta_y(delta_y),
                                    delta_w(delta_w),
                                    delta_h(delta_h){

                                    };
        ~CompositorLayerResizeCommand() override = default;
    };

   struct CompositionRenderCommand : public CompositorCommand {
       CompositionRenderTarget *renderTarget;
       SharedHandle<CanvasFrame> frame;
       CompositionRenderCommand(unsigned id,
                        CompositorClient &client,
                        Type type,
                        Priority priority,
                        decltype(thresholdParams) thresholdParams,
                        OmegaCommon::Promise<CommandStatus> status,
                        CompositionRenderTarget *renderTarget,
                        SharedHandle<CanvasFrame> frame):CompositorCommand(id,client,type,priority,thresholdParams,std::move(status)),
                        renderTarget(renderTarget),
                        frame(frame){

                        }
       ~CompositionRenderCommand() override = default;
   };

   struct CompositorViewCommand : public CompositorCommand {
       typedef enum : int {
           Resize
       } CommandType;
       CommandType subType;
       Native::NativeItemPtr viewPtr;
       unsigned delta_x = 0,delta_y = 0,delta_w = 0,delta_h = 0;
       CompositorViewCommand(unsigned id,
                        CompositorClient &client,
                        Type type,
                        Priority priority,
                        decltype(thresholdParams) thresholdParams,
                        OmegaCommon::Promise<CommandStatus> status,
                        CommandType subType,
                        Native::NativeItemPtr viewPtr,
                        unsigned delta_x,
                        unsigned delta_y,
                        unsigned delta_w,
                        unsigned delta_h):CompositorCommand(id,client,type,priority,thresholdParams,std::move(status)),
                        subType(subType),
                        viewPtr(viewPtr),
                        delta_x(delta_x),
                        delta_y(delta_y),
                        delta_w(delta_w),
                        delta_h(delta_h)
                        {

                        };
       ~CompositorViewCommand() override = default;
   };

   struct CompositorCancelCommand : public CompositorCommand {
       unsigned startID = 0,endID = 0;
       CompositorCancelCommand(unsigned id,
                        CompositorClient &client,
                        Type type,
                        Priority priority,
                        decltype(thresholdParams) thresholdParams,
                        OmegaCommon::Promise<CommandStatus> status,unsigned startID,unsigned endID):CompositorCommand(id,client,type,priority,thresholdParams,std::move(status)),
                        startID(startID),
                        endID(endID){

                        };
       ~CompositorCancelCommand() override = default;
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

    struct ActiveCommandEntry {
        unsigned id;
        OmegaCommon::Async<CommandStatus> status;
    };


    class OMEGAWTK_EXPORT CompositorClient {
        CompositorClientProxy & parentProxy;

        OmegaCommon::Vector<ActiveCommandEntry> currentJobStatuses;

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
