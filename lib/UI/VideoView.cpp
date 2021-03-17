#include "omegaWTK/UI/VideoView.h"
#include "omegaWTK/Composition/ViewRenderTarget.h"

namespace OmegaWTK {

class VideoViewFrameScheduler : public Composition::ViewRenderTargetFrameScheduler {
public:
    VideoViewFrameScheduler();
    void scheduleFrame(float timeDelta){
        
    };
};

VideoView::VideoView():View(<#const Core::Rect &rect#>, <#Composition::LayerTree *layerTree#>),frameScheduler(new VideoViewFrameScheduler()){};

};
