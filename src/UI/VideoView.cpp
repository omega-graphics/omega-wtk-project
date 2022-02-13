#include "omegaWTK/UI/VideoView.h"
#include "omegaWTK/Composition/CompositorClient.h"

#include "omegaWTK/Composition/Canvas.h"

namespace OmegaWTK {



VideoView::VideoView(const Core::Rect & rect,Composition::LayerTree *layerTree,View *parent):View(rect,layerTree,parent),
                                                                                             framebuffer(2){
    videoCanvas = makeCanvas(getLayerTreeLimb()->getRootLayer());
};

void VideoView::queueFrame(SharedHandle<Media::VideoFrame> &frame) {
    SharedHandle<Media::BitmapImage> f(&frame->videoFrame);
    videoCanvas->drawImage(f,getRect());
    f.reset();
    videoCanvas->sendFrame();
}

void VideoView::pushFrame(SharedHandle <Media::VideoFrame> frame) {
    if(!framebuffer.full())
        framebuffer.push(frame);
}

void VideoView::presentCurrentFrame(){
    startCompositionSession();
    auto f = framebuffer.first();
    queueFrame(f);
    framebuffer.pop();
    endCompositionSession();
}

void VideoView::flush() {
    startCompositionSession();
    while(!framebuffer.empty()) {
        auto f = framebuffer.first();
        queueFrame(f);
        framebuffer.pop();
    }
    endCompositionSession();
}

};
