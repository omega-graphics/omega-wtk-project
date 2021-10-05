#include "Audio.h"
#include "Video.h"
#include "AudioVideoProcessorContext.h"

#ifndef OMEGAWTK_MEDIA_MEDIASESSION_H
#define OMEGAWTK_MEDIA_MEDIASESSION_H

namespace OmegaWTK::Media {

    INTERFACE OMEGAWTK_EXPORT AudioPlaybackSession {
    public:
        static SharedHandle<AudioPlaybackSession> Create();
        INTERFACE_METHOD void setAudioSource(AudioInputStream &inputStream) ABSTRACT;
        INTERFACE_METHOD void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void start() ABSTRACT;
        INTERFACE_METHOD void pause() ABSTRACT;
        INTERFACE_METHOD void reset() ABSTRACT;
        INTERFACE_METHOD ~AudioPlaybackSession() = default;
    };

    INTERFACE OMEGAWTK_EXPORT VideoPlaybackSession {
    public:
        static SharedHandle<VideoPlaybackSession> Create();
        INTERFACE_METHOD void setVideoSource(VideoInputStream & inputStream) ABSTRACT;
        INTERFACE_METHOD void setVideoFrameSink(VideoFrameSink & sink) ABSTRACT
        INTERFACE_METHOD void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void start() ABSTRACT;
        INTERFACE_METHOD void pause() ABSTRACT;
        INTERFACE_METHOD void reset() ABSTRACT;
        INTERFACE_METHOD ~VideoPlaybackSession() = default;
    };

}

#endif