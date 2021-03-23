#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_VIDEOCAPTURE_H
#define OMEGAWTK_MEDIA_VIDEOCAPTURE_H

namespace OmegaWTK::Media { 

    class VideoCaptureSession;

    class VideoDevice {
    public:
        virtual UniqueHandle<VideoCaptureSession> createCaptureSession() = 0;
    };

    Core::Vector<VideoDevice *> enumerateVideoDevices();

    class VideoCaptureSession {
    public:
        virtual void startPreview() = 0;
        virtual void startRecord() = 0;
        virtual void stopRecord() = 0;
        virtual void stopPreview() = 0;
    };
};

#endif