#include "omegaWTK/Core/Core.h"

#include "MediaIO.h"

#ifndef OMEGAWTK_MEDIA_AUDIO_H
#define OMEGAWTK_MEDIA_AUDIO_H

namespace OmegaWTK::Media {
    class AudioCaptureSession;

    /// @interface AudioCaptureDevice
    /// @brief A physical device for capturing audio. (e.g. a USB microphone)
    class AudioCaptureDevice {
    public:
        /** @brief Creates a Capture Session for this device.
           @returns A new AudioCaptureSession.
         */
        INTERFACE_METHOD UniqueHandle<AudioCaptureSession> createCaptureSession() ABSTRACT;
    };

    /** @brief Enumerate all currently active and discoverable devices for audio capture.
       @returns OmegaCommon::Vector<AudioCaptureDevice>
     */
    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> enumerateAudioCaptureDevices();

    /// @brief A physical device for playing audio. (e.g. a USB speaker)
    class AudioPlaybackDevice;

    /** @brief Enumerate all currently active and discoverable devices for audio playback.
        @returns OmegaCommon::Vector<AudioPlaybackDevice>
     */
    OMEGAWTK_EXPORT OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> enumerateAudioPlaybackDevices();

    /// @interface AudioCaptureSession
    /// @brief A temporary context for capturing audio input from a capture device.
    class AudioCaptureSession {
    public:

        /** @brief Sets the AudioPlaybackDevice to receive preview audio samples.
           @param device The Playback Device to use.
         */
        INTERFACE_METHOD void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> & device) ABSTRACT;

        /** @brief Sets the output stream.
           @param outputStream The MediaOutputStream to output audio samples to.
         */
        INTERFACE_METHOD void setAudioOutputStream(MediaOutputStream & outputStream) ABSTRACT;

        /// @brief Starts previewing audio samples from capture device. (Not capturing output).
        INTERFACE_METHOD void startPreview() ABSTRACT;

        /// @brief Starts capturing audio samples from capture device.
        INTERFACE_METHOD void startRecord() ABSTRACT;

        /// @brief Stops capturing audio samples from capture device.
        INTERFACE_METHOD void stopRecord() ABSTRACT;

        /// @brief Stops previewing audio samples from capture device.
        INTERFACE_METHOD void stopPreview() ABSTRACT;
    };
};

#endif