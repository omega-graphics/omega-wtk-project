#include "omegaWTK/Composition/ViewRenderTarget.h"

namespace OmegaWTK::Composition {
    
    

    ViewRenderTarget::ViewRenderTarget(Native::NativeItemPtr _native) : native(_native){};
    Native::NativeItemPtr ViewRenderTarget::getNativePtr(){ return native;};
    ViewRenderTarget::~ViewRenderTarget(){};

ViewRenderTargetFrameScheduler::ViewRenderTargetFrameScheduler(Core::UniquePtr<ViewRenderTarget> & ptr,
                                                               Composition::Compositor * comp):
renderTarget(ptr),compositor(comp){
    
};
}
