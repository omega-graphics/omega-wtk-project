#include "omegaWTK/Core/Core.h"

#include "Audio.h"
#include "AudioVideoProcessorContext.h"

#ifndef OMEGAWTK_MEDIA_VIDEOCAPTURE_H
#define OMEGAWTK_MEDIA_VIDEOCAPTURE_H

namespace OmegaWTK::Media {

    INTERFACE VideoCaptureSession;

    INTERFACE OMEGAWTK_EXPORT VideoDevice {
    public:
        INTERFACE_METHOD UniqueHandle<VideoCaptureSession> createCaptureSession() ABSTRACT;
    };

    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<VideoDevice>> enumerateVideoDevices();

    typedef std::istream VideoInputStream;
    typedef std::ostream VideoOutputStream;

    INTERFACE VideoCaptureSession {
    public:
        INTERFACE_METHOD void setVideoFrameSinForPreview(VideoFrameSink &frameSink) ABSTRACT;
        INTERFACE_METHOD void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;
        INTERFACE_METHOD void setVideoOutputStream(VideoOutputStream &outputStream) ABSTRACT;
        INTERFACE_METHOD void startPreview() ABSTRACT;
        INTERFACE_METHOD void startRecord() ABSTRACT;
        INTERFACE_METHOD void stopRecord() ABSTRACT;
        INTERFACE_METHOD void stopPreview() ABSTRACT;
    };
};

#endif