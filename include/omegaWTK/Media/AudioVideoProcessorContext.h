#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Media/ImgCodec.h"

#include <chrono>

#ifndef OMEGAWTK_MEDIA_AUDIOVIDEOPROCESSORCONTEXT_H
#define  OMEGAWTK_MEDIA_AUDIOVIDEOPROCESSORCONTEXT_H

namespace OmegaWTK {
    namespace Media {

        typedef std::chrono::high_resolution_clock::time_point TimePoint;

        struct VideoFrame {
            BitmapImage videoFrame;
            TimePoint decodeFinishTime;
            TimePoint presentTime;
        };

        INTERFACE VideoFrameSink {
        public:
            INTERFACE_METHOD bool framebuffered() const ABSTRACT;
            INTERFACE_METHOD void pushFrame(SharedHandle<VideoFrame> frame) ABSTRACT;
            INTERFACE_METHOD void presentCurrentFrame() ABSTRACT;
            INTERFACE_METHOD void flush() ABSTRACT;
        };

        class AudioVideoProcessor;

        OMEGAWTK_EXPORT UniqueHandle<AudioVideoProcessor> createAudioVideoProcessor(bool useHardwareAccel,void *gteDevice);

        class AudioVideoProcessorContext {
        protected:
            AudioVideoProcessor *processor;
            explicit AudioVideoProcessorContext(UniqueHandle<AudioVideoProcessor> & processor){
                this->processor = processor.get();
            };
        };
    };
};

#endif