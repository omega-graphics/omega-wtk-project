#include "omegaWTK/Core/Core.h"
#include "omegaWTK/Media/AudioVideoProcessorContext.h"

namespace OmegaWTK::Media {

    enum class Codec : int {
        MPEG4,
        H264,
        H265
    };

    struct EncodedAudioVideoFrame {
       void *encoded_core_data;
       size_t core_data_size;
    };

    INTERFACE AudioVideoProcessor {
    public:
        struct BeginEncodeParams {
            Codec codec;
        };
        INTERFACE_METHOD(void,beginEncode,BeginEncodeParams params)
        INTERFACE_METHOD(void,endEncode)

        struct BeginDecodeParams {
            Codec codec;
        };
        INTERFACE_METHOD(void,beginDecode,BeginDecodeParams params)
        INTERFACE_METHOD(void,endDecode)

        struct EncodeFrameParams {
            AudioVideoFrame & src;
            EncodedAudioVideoFrame & dest;
        };
        INTERFACE_METHOD(void,encodeFrame,EncodeFrameParams params)

        struct DecodeFrameParams {
            EncodedAudioVideoFrame & src;
            AudioVideoFrame & dest;
        };
        INTERFACE_METHOD(void,decodeFrame,DecodeFrameParams params)

        static void Initialize();
        static void Cleanup();

        virtual ~AudioVideoProcessor() = default;
    };

    extern Core::UniquePtr<AudioVideoProcessor> avProcessor;

    
};