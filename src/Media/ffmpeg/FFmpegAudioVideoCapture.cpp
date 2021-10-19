#include "omegaWTK/Media/Audio.h"
#include "omegaWTK/Media/Video.h"
#include "omegaWTK/Media/AudioVideoProcessorContext.h"


#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>


namespace OmegaWTK::Media {
    class AudioPlaybackDevice {
        AVFormatContext device;
    public:
        AudioPlaybackDevice(){

        }
        ~AudioPlaybackDevice(){

        }
    };

    class FFmpegVideoDevice : public VideoDevice {
        AVFormatContext device;
    public:
        UniqueHandle<VideoCaptureSession> createCaptureSession() override {

        }
    };
}