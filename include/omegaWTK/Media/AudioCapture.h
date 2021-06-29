#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_AUDIOCAPTURE_H
#define OMEGAWTK_MEDIA_AUDIOCAPTURE_H

namespace OmegaWTK::Media {
    class AudioCaptureSession;

    class AudioDevice {
    public:
        virtual UniqueHandle<AudioCaptureSession> createCaptureSession() = 0;
    };

    OmegaCommon::Vector<AudioDevice *> enumerateAudioDevices();

    class AudioCaptureSession {
    public:
        virtual void startPreview() = 0;
        virtual void startRecord() = 0;
        virtual void stopRecord() = 0;
        virtual void stopPreview() = 0;
    };
};

#endif