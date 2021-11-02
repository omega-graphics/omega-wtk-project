#include "omegaWTK/Core/Core.h"

#include "Audio.h"
#include "AudioVideoProcessorContext.h"

#ifndef OMEGAWTK_MEDIA_VIDEO_H
#define OMEGAWTK_MEDIA_VIDEO_H

namespace OmegaWTK::Media {

    INTERFACE VideoCaptureSession;

    INTERFACE OMEGAWTK_EXPORT VideoDevice {
    public:
        INTERFACE_METHOD UniqueHandle<VideoCaptureSession> createCaptureSession(SharedHandle<AudioCaptureDevice> & audioCaptureDevice) ABSTRACT;
    };

    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<VideoDevice>> enumerateVideoDevices();


    INTERFACE VideoCaptureSession {
    public:
        INTERFACE_METHOD void setVideoFrameSinkForPreview(VideoFrameSink &frameSink) ABSTRACT;
        INTERFACE_METHOD void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void setVideoOutputStream(MediaOutputStream &outputStream) ABSTRACT;
        INTERFACE_METHOD void startPreview() ABSTRACT;
        INTERFACE_METHOD void startRecord() ABSTRACT;
        INTERFACE_METHOD void stopRecord() ABSTRACT;
        INTERFACE_METHOD void stopPreview() ABSTRACT;
    };
};

#endif