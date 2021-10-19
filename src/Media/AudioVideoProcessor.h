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
        INTERFACE_METHOD void beginEncode(BeginEncodeParams params) ABSTRACT;
        INTERFACE_METHOD void endEncode()ABSTRACT;

        struct BeginDecodeParams {
            Codec codec;
        };
        INTERFACE_METHOD void beginDecode(BeginDecodeParams params)ABSTRACT;
        INTERFACE_METHOD void endDecode()ABSTRACT;

        struct EncodeFrameParams {
            AudioVideoFrame & src;
            EncodedAudioVideoFrame & dest;
        };
        INTERFACE_METHOD void encodeFrame(EncodeFrameParams params)ABSTRACT;

        struct DecodeFrameParams {
            EncodedAudioVideoFrame & src;
            AudioVideoFrame & dest;
        };
        INTERFACE_METHOD void decodeFrame(DecodeFrameParams params)ABSTRACT;

        static void Initialize();
        static void Cleanup();

        virtual ~AudioVideoProcessor() = default;
    };

    extern Core::UniquePtr<AudioVideoProcessor> avProcessor;

    
};