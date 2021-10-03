#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Media/ImgCodec.h"

#ifndef OMEGAWTK_MEDIA_AUDIOVIDEOPROCESSORCONTEXT_H
#define  OMEGAWTK_MEDIA_AUDIOVIDEOPROCESSORCONTEXT_H

namespace OmegaWTK {
    namespace Media {

        struct AudioVideoFrame {
            BitmapImage videoFrame;
            struct {
                void *data;
                size_t length;
            } audioFrame;
        };

        INTERFACE VideoFrameSink {

        };

        class AudioVideoProcessor;

        class AudioVideoProcessorContext {
            AudioVideoProcessor *processor;
        protected:
            void setFrameSink(VideoFrameSink & sink);
        };

    };
};

#endif