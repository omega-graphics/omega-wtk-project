#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_VIDEOCAPTURE_H
#define OMEGAWTK_MEDIA_VIDEOCAPTURE_H

namespace OmegaWTK::Media { 

    class VideoCaptureSession;

    class OMEGAWTK_EXPORT VideoDevice {
    public:
        virtual UniqueHandle<VideoCaptureSession> createCaptureSession() = 0;
    };

    Core::Vector<VideoDevice *> enumerateVideoDevices();
    class VideoCaptureSessionPreviewOutput {

    };
    class VideoCaptureSession {
    public:
        virtual void startPreview(VideoCaptureSessionPreviewOutput & output) = 0;
        virtual void startRecord() = 0;
        virtual void stopRecord() = 0;
        virtual void stopPreview() = 0;
    };
};

#endif