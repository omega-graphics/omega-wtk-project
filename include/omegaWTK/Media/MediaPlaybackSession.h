#include "Audio.h"
#include "Video.h"
#include "AudioVideoProcessorContext.h"

#ifndef OMEGAWTK_MEDIA_MEDIASESSION_H
#define OMEGAWTK_MEDIA_MEDIASESSION_H

namespace OmegaWTK::Media {

    /// @brief A threaded queue for scheduling and dispatching playback of media from playback sessions.
    class PlaybackDispatchQueue;

    /** @brief Creates A PlaybackDispatchQueue
       @returns A new PlaybackDispatchQueue.
     */
    OMEGAWTK_EXPORT SharedHandle<PlaybackDispatchQueue> createPlaybackDispatchQueue();

    INTERFACE OMEGAWTK_EXPORT AudioPlaybackSession : public AudioVideoProcessorContext{
    protected:
        explicit AudioPlaybackSession(UniqueHandle<AudioVideoProcessor> & processor) : AudioVideoProcessorContext(processor){}
    public:
        static SharedHandle<AudioPlaybackSession> Create(UniqueHandle<AudioVideoProcessor> & processor,
                                                         SharedHandle<PlaybackDispatchQueue> & dispatchQueue);
        INTERFACE_METHOD void setAudioSource(MediaInputStream &inputStream) ABSTRACT;
        INTERFACE_METHOD void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void start() ABSTRACT;
        INTERFACE_METHOD void pause() ABSTRACT;
        INTERFACE_METHOD void reset() ABSTRACT;
        INTERFACE_METHOD ~AudioPlaybackSession() = default;
    };

    INTERFACE OMEGAWTK_EXPORT VideoPlaybackSession  : public AudioVideoProcessorContext{
    protected:
        explicit VideoPlaybackSession(UniqueHandle<AudioVideoProcessor> & processor) : AudioVideoProcessorContext(processor){}
    public:
        static SharedHandle<VideoPlaybackSession> Create(UniqueHandle<AudioVideoProcessor> & processor,
                                                         SharedHandle<PlaybackDispatchQueue> & dispatchQueue);
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