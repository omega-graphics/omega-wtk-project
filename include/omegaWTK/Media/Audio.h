#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_AUDIOCAPTURE_H
#define OMEGAWTK_MEDIA_AUDIOCAPTURE_H

namespace OmegaWTK::Media {
    class AudioCaptureSession;

    class AudioCaptureDevice {
    public:
        virtual UniqueHandle<AudioCaptureSession> createCaptureSession() = 0;
    };

    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> enumerateAudioCaptureDevices();

    class AudioPlaybackDevice;

    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> enumerateAudioPlaybackDevices();

    typedef std::istream AudioInputStream;
    typedef std::ostream AudioOutputStream;

    class AudioCaptureSession {
    public:
        virtual void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> & device) = 0;
        virtual void setAudioOutputStream(AudioOutputStream & outputStream) = 0;
        virtual void startPreview() = 0;
        virtual void startRecord() = 0;
        virtual void stopRecord() = 0;
        virtual void stopPreview() = 0;
    };
};

#endif