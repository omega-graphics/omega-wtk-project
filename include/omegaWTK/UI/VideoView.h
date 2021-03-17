#include "View.h"

#ifndef OMEGAWTK_UI_VIDEOVIEW_H
#define OMEGAWTK_UI_VIDEOVIEW_H

namespace OmegaWTK {

class VideoViewFrameScheduler;

class OMEGAWTK_EXPORT VideoView : public View {
    VideoViewFrameScheduler *frameScheduler;
public:
    VideoView();
};

};

#endif
