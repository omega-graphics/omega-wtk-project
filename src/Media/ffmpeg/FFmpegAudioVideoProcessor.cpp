#include "../AudioVideoProcessor.h"

#include <libavcodec/avcodec.h>

namespace OmegaWTK::Media {
    class FFmpegAudioVideoProcessor : public AudioVideoProcessor {
    public:
        void beginEncode() override{
            
        }
    
        void encodeFrame() override{

        }

        void endEncode() override{

        }

        void beginDecode() override{

        }

        void decodeFrame() override {

        }

        void endDecode() override{

        }

        FFmpegAudioVideoProcessor(){
            
        };
        ~FFmpegAudioVideoProcessor(){

        };
    };

    Core::UniquePtr<AudioVideoProcessor> avProcessor;
};