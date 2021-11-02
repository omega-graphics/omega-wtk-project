#include "Audio.h"
#include "Video.h"
#include "AudioVideoProcessorContext.h"

#ifndef OMEGAWTK_MEDIA_MEDIASESSION_H
#define OMEGAWTK_MEDIA_MEDIASESSION_H

namespace OmegaWTK::Media {

    class PlaybackDispatchQueue;
    OMEGAWTK_EXPORT SharedHandle<PlaybackDispatchQueue> createPlaybackDispatchQueue();

    INTERFACE OMEGAWTK_EXPORT AudioPlaybackSession : public AudioVideoProcessorContext{
    public:
        static SharedHandle<AudioPlaybackSession> Create(SharedHandle<PlaybackDispatchQueue> & dispatchQueue);
        INTERFACE_METHOD void setAudioSource(MediaInputStream &inputStream) ABSTRACT;
        INTERFACE_METHOD void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void start() ABSTRACT;
        INTERFACE_METHOD void pause() ABSTRACT;
        INTERFACE_METHOD void reset() ABSTRACT;
        INTERFACE_METHOD ~AudioPlaybackSession() = default;
    };

    INTERFACE OMEGAWTK_EXPORT VideoPlaybackSession  : public AudioVideoProcessorContext{
    public:
        static SharedHandle<VideoPlaybackSession> Create(SharedHandle<PlaybackDispatchQueue> & dispatchQueue);
        INTERFACE_METHOD void setVideoSource(MediaInputStream & inputStream) ABSTRACT;
        INTERFACE_METHOD void setVideoFrameSink(VideoFrameSink & sink) ABSTRACT
        INTERFACE_METHOD void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void start() ABSTRACT;
        INTERFACE_METHOD void pause() ABSTRACT;
        INTERFACE_METHOD void reset() ABSTRACT;
        INTERFACE_METHOD ~VideoPlaybackSession() = default;
    };

}

#endif