#include "omegaWTK/Composition/CompositorClient.h"
#include "omegaWTK/Composition/Compositor.h"

//#ifdef TARGET_MACOS
//#include "CocoaRenderTargetFrameScheduler.h"
//#endif

// #ifdef TARGET_WIN32
// #include "WinRenderTargetFrameScheduler.h"
// #endif

namespace OmegaWTK::Composition {
    
    // CompositorClient::CompositorClient(){

    // };
    void CompositorClient::queueVisualCommand(VisualCommand *v){
        drawQueue.push_back(*v);
    };

    void CompositorClient::submit(CompositionRenderTarget *renderTarget){
        auto command = std::make_unique<CompositionRenderCommand>();
        command->renderTarget = renderTarget;
        unsigned visual_c = drawQueue.size();
        std::move(drawQueue.begin(),drawQueue.end(),command->_visuals);
        drawQueue.clear();
        command->priority = CompositionRenderCommand::Low;
        command->thresholdParams.hasThreshold = false;
        command->thresholdParams.timeStamp = std::chrono::high_resolution_clock::now();
        command->visual_count = visual_c;
        frontend->scheduleCommand(std::move(command));
    };

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
