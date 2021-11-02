#include "omegaWTK/Core/Core.h"

#include "MediaIO.h"

#ifndef OMEGAWTK_MEDIA_AUDIO_H
#define OMEGAWTK_MEDIA_AUDIO_H

namespace OmegaWTK::Media {
    class AudioCaptureSession;

    class AudioCaptureDevice {
    public:
        virtual UniqueHandle<AudioCaptureSession> createCaptureSession() = 0;
    };

    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> enumerateAudioCaptureDevices();

    class AudioPlaybackDevice;

    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> enumerateAudioPlaybackDevices();

    class AudioCaptureSession {
    public:
        virtual void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> & device) = 0;
        virtual void setAudioOutputStream(MediaOutputStream & outputStream) = 0;
        virtual void startPreview() = 0;
        virtual void startRecord() = 0;
        virtual void stopRecord() = 0;
        virtual void stopPreview() = 0;
    };
};

#endif