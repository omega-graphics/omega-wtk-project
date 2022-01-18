#include "View.h"
#include "omegaWTK/Media/Video.h"

#ifndef OMEGAWTK_UI_VIDEOVIEW_H
#define OMEGAWTK_UI_VIDEOVIEW_H

namespace OmegaWTK {

/**
 @brief The visual display output of a VideoPlaybackSession or a capture preview output of a VideoCaptureSession.
*/
class OMEGAWTK_EXPORT VideoView :  public View, 
                                   public Media::VideoFrameSink {

    OmegaCommon::QueueHeap<SharedHandle<Media::VideoFrame>> framebuffer;

    SharedHandle<Composition::Canvas> videoCanvas;

    void queueFrame(SharedHandle<Media::VideoFrame> &frame);

    bool framebuffered() const override {
        return true;
    };
    void flush() override;
    void pushFrame(SharedHandle<Media::VideoFrame> frame) override;
    void presentCurrentFrame() override;
public:
    OMEGACOMMON_CLASS("OmegaWTK.VideoView")
    friend class Widget;

   VideoView(const Core::Rect & rect,Composition::LayerTree * layerTree,View *parent = nullptr);
};
/**
 @brief 
*/

};

#endif
