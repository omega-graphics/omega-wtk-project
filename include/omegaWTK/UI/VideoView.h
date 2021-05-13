#include "View.h"
#include "omegaWTK/Media/VideoCapture.h"
#include "omegaWTK/Media/VideoCodec.h"

#ifndef OMEGAWTK_UI_VIDEOVIEW_H
#define OMEGAWTK_UI_VIDEOVIEW_H

namespace OmegaWTK {

class VideoViewController;
/**
 @brief 
*/
class OMEGAWTK_EXPORT VideoView : public View, public Media::VideoCaptureSessionPreviewOutput {
    VideoViewController *controller;
    Media::VideoIStream *currentStream;
public:
    void setVideoInput(Media::VideoIStream &videoIn);
   void setController(VideoViewController *controller);
   VideoView(const Core::Rect & rect,Composition::Compositor * comp,View *parent = nullptr);
};
/**
 @brief 
*/
class OMEGAWTK_EXPORT VideoViewController {
    VideoView *view;
    bool hasAssignment;
public:
    void seek();
    void play();
    void pause();
};

};

#endif
