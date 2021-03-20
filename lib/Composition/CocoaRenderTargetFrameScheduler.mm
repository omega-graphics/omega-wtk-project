#include "CocoaRenderTargetFrameScheduler.h"

#include "NativePrivate/macos/CocoaItem.h"

#import <Cocoa/Cocoa.h>
#import <CoreVideo/CoreVideo.h>

namespace OmegaWTK::Composition {

//class CocoaRenderTargetFrameSchedulerImpl : public CocoaRenderTargetFrameScheduler {
//    Native::Cocoa::CocoaItem *cocoaItem;
//    CVDisplayLinkRef cv_display_link_ref;
//public:
//    CocoaRenderTargetFrameSchedulerImpl(Native::Cocoa::CocoaItem *native,Compositor *comp):CocoaRenderTargetFrameScheduler(comp){
//        CVDisplayLinkCreateWithActiveCGDisplays(&cv_display_link_ref);
//        CVDisplayLinkStart(cv_display_link_ref);
//    };
//    ~CocoaRenderTargetFrameSchedulerImpl(){
//        CVDisplayLinkStop(cv_display_link_ref);
//        CVDisplayLinkRelease(cv_display_link_ref);
//    };
//};
//
//CocoaRenderTargetFrameScheduler::CocoaRenderTargetFrameScheduler(Compositor *comp):ViewRenderTargetFrameScheduler(comp){
//    
//};

//Core::UniquePtr<ViewRenderTargetFrameScheduler> CocoaRenderTargetFrameScheduler::Create(Core::UniquePtr<ViewRenderTarget> & ptr,Compositor *comp){
//    return std::make_unique<CocoaRenderTargetFrameSchedulerImpl>((Native::Cocoa::CocoaItem *)ptr->getNativePtr(),comp);
//};

}
