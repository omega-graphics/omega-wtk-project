#include "omegaWTK/Media/VideoCodec.h"
#import <CoreVideo/CoreVideo.h>
#import <CoreAudio/CoreAudio.h>
#import <CoreMedia/CoreMedia.h>
#import <AudioToolbox/AudioToolbox.h>
#import <VideoToolbox/VideoToolbox.h>


namespace OmegaWTK::Media {
    class VideoToolboxCodec {
    public:
    };

    VideoIStream decode_video(void *data, size_t data_size, VideoCodecType type){

    };

    void encode_video(VideoOStream ostream, VideoCodecType type){

    };
};