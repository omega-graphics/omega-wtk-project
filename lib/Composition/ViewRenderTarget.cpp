#include "omegaWTK/Composition/ViewRenderTarget.h"

#ifdef TARGET_MACOS
#include "CocoaRenderTargetFrameScheduler.h"
#endif

#ifdef TARGET_WIN32
#include "WinRenderTargetFrameScheduler.h"
#endif

namespace OmegaWTK::Composition {
    
    

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
