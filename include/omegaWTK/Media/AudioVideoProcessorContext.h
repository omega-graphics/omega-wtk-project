#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_AUDIOVIDEOPROCESSORCONTEXT_H
#define  OMEGAWTK_MEDIA_AUDIOVIDEOPROCESSORCONTEXT_H

namespace OmegaWTK {
    namespace Media {


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