#include "../AudioVideoProcessor.h"

#import <AVFoundation/AVFoundation.h>
#import <AVFAudio/AVFAudio.h>
#import <CoreVideo/CoreVideo.h>
#import <VideoToolbox/VideoToolbox.h>

namespace OmegaWTK::Media {
    class AVFAudioVideoProcessor : public AudioVideoProcessor {

        VTCompressionSessionRef compSession;

        VTDecompressionSessionRef decompSession;

    public:
        void beginEncode(BeginEncodeParams params) override{
            VTCompressionSessionCreate(NULL,0,0,kCMVideoCodecType_HEVC,NULL,NULL,NULL,NULL,NULL,&compSession);
        }
    
        void encodeFrame(EncodeFrameParams params) override{
            CVPixelBufferRef pixelBuffer;
//            CVPixelBufferCreateWithBytes()
            CMTime time;
            VTCompressionSessionEncodeFrame(compSession,pixelBuffer,time,time,NULL,params.dest.encoded_core_data,NULL);

        }

        void endEncode() override{
            VTCompressionSessionInvalidate(compSession);
            CFRelease(compSession);
        }   

        void beginDecode(BeginDecodeParams params) override{
            VTDecompressionSessionCreate(NULL,NULL,NULL,NULL,NULL,&decompSession);
        }

        void decodeFrame(DecodeFrameParams params) override {
            CMSampleBufferRef sampleBuffer;
            VTDecompressionSessionDecodeFrame(decompSession,sampleBuffer,kVTDecodeFrame_1xRealTimePlayback,NULL,NULL);

        }

        void endDecode() override{
            VTDecompressionSessionInvalidate(decompSession);
            CFRelease(compSession);
        }

        AVFAudioVideoProcessor() = default;
        ~AVFAudioVideoProcessor() = default;
    };


   Core::UniquePtr<AudioVideoProcessor> avProcessor;

    void AudioVideoProcessor::Initialize() {
        avProcessor.reset(new AVFAudioVideoProcessor());
    }

    void AudioVideoProcessor::Cleanup() {
        avProcessor.reset();
    }
};