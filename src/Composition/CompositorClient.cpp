#include "omegaWTK/Composition/CompositorClient.h"
#include "Compositor.h"

namespace OmegaWTK::Composition {
    
    // CompositorClient::CompositorClient(){

    // };
    void CompositorClientProxy::beginRecord() {
        assert(!recording && "Client proxy must not be recording before starting a new recording session");
        recording = true;
    }

    void CompositorClientProxy::endRecord() {
        assert(recording && "Client Proxy must be recording commands before ending");
        recording = false;
        submit();
    }

   OmegaCommon::Async<CommandStatus> CompositorClientProxy::queueTimedFrame(unsigned & id,
                                                                            CompositorClient & client,
                                                                            SharedHandle<CanvasFrame> &frame,
                                                                            Timestamp &start,
                                                                            Timestamp &deadline){
       OmegaCommon::Promise<CommandStatus> status;
       auto async = status.async();
        commandQueue.emplace(new
                        CompositionRenderCommand{id,client,CompositorCommand::Type::Render,
                                 CompositorCommand::Priority::High,
                                 {true,start,deadline},std::move(status),renderTarget,frame});
        return async;
    };

    OmegaCommon::Async<CommandStatus> CompositorClientProxy::queueFrame(unsigned & id,
                                           CompositorClient & client,
                                           SharedHandle<CanvasFrame> & frame,
                                           Timestamp &start){
        OmegaCommon::Promise<CommandStatus> status;
        auto async = status.async();
        commandQueue.emplace(
            new
            CompositionRenderCommand{
                                id,client,CompositorCommand::Type::Render,
                                 CompositorCommand::Priority::Low,
                                 {false,start,start},std::move(status),renderTarget,frame});
        return async;
    };

    OmegaCommon::Async<CommandStatus> CompositorClientProxy::queueViewResizeCommand(unsigned & id,
                                                       CompositorClient & client,
                                                       Native::NativeItemPtr nativeView,
                                                       unsigned int delta_x,
                                                       unsigned int delta_y,
                                                       unsigned int delta_w,
                                                       unsigned int delta_h,
                                                       Timestamp &start,
                                                       Timestamp &deadline) {
        OmegaCommon::Promise<CommandStatus> status;
        auto async = status.async();
        commandQueue.emplace(new CompositorViewCommand{
            id,
            client,
            CompositorCommand::Type::View,
            CompositorCommand::Priority::High,
            {true,start,deadline},
            std::move(status),
            CompositorViewCommand::Resize,nativeView,delta_x,delta_y,delta_w,delta_h});
        return async;
    }

    OmegaCommon::Async<CommandStatus> CompositorClientProxy::queueLayerResizeCommand(unsigned & id,
                                                                                     CompositorClient & client,
                                                                                     Layer *target,
                                                                                     unsigned int delta_x,
                                                                                     unsigned int delta_y,
                                                                                     unsigned int delta_w,
                                                                                     unsigned int delta_h,
                                                                                     Timestamp &start,
                                                                                     Timestamp &deadline) {
        OmegaCommon::Promise<CommandStatus> status;
        auto async = status.async();

        commandQueue.emplace(new CompositorLayerResizeCommand {
            id,
            client,
            CompositorCommand::Type::LayerResize,
            CompositorCommand::Priority::High,
            {true,start,deadline},std::move(status),target,delta_x,delta_y,delta_w,delta_h});
        return async;
    }

    OmegaCommon::Async<CommandStatus> CompositorClientProxy::queueCancelCommand(unsigned & id,
                                                                                CompositorClient & client,
                                                                                unsigned startID,
                                                                                unsigned endID) {
        Timestamp ts = std::chrono::high_resolution_clock::now();
        OmegaCommon::Promise<CommandStatus> status;
        auto async = status.async();
        commandQueue.emplace(new CompositorCancelCommand {
            id,
            client,
            CompositorCommand::Type::Cancel,
            CompositorCommand::Priority::High,
            {false,ts,ts},std::move(status),startID,endID});
        return async;
    }

    void CompositorClientProxy::setFrontendPtr(Compositor *frontend){
        this->frontend = frontend;
    };

    void CompositorClientProxy::submit(){
       while(!commandQueue.empty()){
           auto comm = commandQueue.front();
           frontend->scheduleCommand(comm);
           commandQueue.pop();
       }
    };

    CompositorClient::CompositorClient(CompositorClientProxy &proxy):
    parentProxy(proxy),
    currentCommandID(0){

    }

    void CompositorClient::pushLayerResizeCommand(Layer *target, unsigned int delta_x, unsigned int delta_y,
                                                  unsigned int delta_w, unsigned int delta_h, Timestamp &start,
                                                  Timestamp &deadline) {
        currentJobStatuses.push_back({
            currentCommandID,
            parentProxy.queueLayerResizeCommand(currentCommandID,*this,target,delta_x,delta_y,delta_w,delta_h,start,deadline)
            });
        ++currentCommandID;
    }

    void CompositorClient::pushTimedFrame(SharedHandle<CanvasFrame> &frame, Timestamp &start, Timestamp &deadline) {
        currentJobStatuses.push_back(
            {currentCommandID,
            parentProxy.queueTimedFrame(currentCommandID,*this,frame,start,deadline)
            });
        ++currentCommandID;
    }

    void CompositorClient::pushViewResizeCommand(Native::NativeItemPtr nativeView,unsigned delta_x,unsigned delta_y,unsigned delta_w,unsigned delta_h,Timestamp &start,Timestamp & deadline){
        currentJobStatuses.push_back(
            {currentCommandID,
            parentProxy.queueViewResizeCommand(currentCommandID,*this,nativeView,delta_x,delta_y,delta_w,delta_h,start,deadline)
            });
        ++currentCommandID;
    }

    void CompositorClient::pushFrame(SharedHandle<CanvasFrame> &frame, Timestamp &start) {
        currentJobStatuses.push_back({currentCommandID,
                                    parentProxy.queueFrame(currentCommandID,*this,frame,start)
                                                    });
        ++currentCommandID;
    }

    void CompositorClient::cancelCurrentJobs() {
        if(busy()) {
            auto idStart = currentJobStatuses.front().id;
            auto idEnd = currentJobStatuses.back().id;
            currentJobStatuses.push_back({currentCommandID,
                                            parentProxy.queueCancelCommand(currentCommandID, *this, idStart,idEnd)
                                            });
            ++currentCommandID;
        }
    }

    bool CompositorClient::busy() {
        OmegaCommon::Vector<unsigned> jobsToDelete;
        for(unsigned i = 0;i < currentJobStatuses.size();i++){
            auto & job = currentJobStatuses[i];
            if(job.status.ready()){
                jobsToDelete.push_back(i);
            }
        }

        for(auto & j : jobsToDelete){
            currentJobStatuses.erase(currentJobStatuses.begin() + j);
        }

        return !currentJobStatuses.empty();
    }

    ViewRenderTarget::ViewRenderTarget(Native::NativeItemPtr _native) : native(_native){};
    Native::NativeItemPtr ViewRenderTarget::getNativePtr(){ return native;};
    ViewRenderTarget::~ViewRenderTarget(){};

////ViewRenderTargetFrameScheduler::ViewRenderTargetFrameScheduler(Composition::Compositor * comp):
////compositor(comp){
////    
////};
//
//Core::UniquePtr<ViewRenderTargetFrameScheduler> ViewRenderTargetFrameScheduler::Create(Core::UniquePtr<ViewRenderTarget> & ptr,Compositor * comp){
//    
//};
}
