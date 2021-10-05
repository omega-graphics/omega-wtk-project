#include "omegaWTK/Composition/CompositorClient.h"
#include "Compositor.h"

namespace OmegaWTK::Composition {
    
    // CompositorClient::CompositorClient(){

    // };
    void CompositorClient::queueTimedFrame(SharedHandle<CanvasFrame> &frame, Timestamp &start, Timestamp &deadline){
        commandQueue.emplace(new
        CompositionRenderCommand{CompositorCommand::Type::Render,
                                 CompositorCommand::Priority::High,
                                 {true,start,deadline},renderTarget,frame});
    };

    void CompositorClient::queueFrame(SharedHandle<CanvasFrame> & frame,Timestamp &start){
        commandQueue.emplace(new
        CompositionRenderCommand{CompositorCommand::Type::Render,
                                 CompositorCommand::Priority::Low,
                                 {false,start,start},renderTarget,frame});
    };

    void CompositorClient::queueViewResizeCommand(Native::NativeItemPtr nativeView, unsigned int delta_x,
                                                  unsigned int delta_y, unsigned int delta_w, unsigned int delta_h,
                                                  Timestamp &start, Timestamp &deadline) {
        commandQueue.emplace(new CompositorViewCommand {
            CompositorCommand::Type::View,
            CompositorCommand::Priority::High,
            {true,start,deadline},
            CompositorViewCommand::Resize,nativeView,delta_x,delta_y,delta_w,delta_h});
    }

    void CompositorClient::queueLayerResizeCommand(Layer *target, unsigned int delta_x, unsigned int delta_y,
                                                   unsigned int delta_w, unsigned int delta_h, Timestamp &start,
                                                   Timestamp &deadline) {
        commandQueue.emplace(new CompositorLayerResizeCommand {
                CompositorCommand::Type::LayerResize,
                CompositorCommand::Priority::High,
                {true,start,deadline},target,delta_x,delta_y,delta_w,delta_h});
    }

    void CompositorClient::queueStopForRenderingLayer(Layer *target) {
        Timestamp ts = std::chrono::high_resolution_clock::now();
        commandQueue.emplace(new CompositorHoldRenderCommand {
                CompositorCommand::Type::HoldRender,
                CompositorCommand::Priority::High,
                {false,ts,ts},target});
    }

    void CompositorClient::queueResumeForRenderingLayer(Layer *target) {
        Timestamp ts = std::chrono::high_resolution_clock::now();
        commandQueue.emplace(new CompositorResumeRenderCommand {
                CompositorCommand::Type::ResumeRender,
                CompositorCommand::Priority::High,
                {false,ts,ts},target});
    }

    void CompositorClient::setFrontendPtr(Compositor *frontend){
        this->frontend = frontend;
    };

    void CompositorClient::submit(){
       while(!commandQueue.empty()){
           auto comm = commandQueue.front();

           frontend->scheduleCommand(comm);
           commandQueue.pop();
       }
    };

    CompositorClient::~CompositorClient() = default;

//    CompositorVideoClient::~CompositorVideoClient(){
//
//    };

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
