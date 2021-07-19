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
        void beginEncode() override{
            VTCompressionSessionCreate(NULL,0,0,kCMVideoCodecType_HEVC,NULL,NULL,NULL,NULL,NULL,&compSession);
        }
    
        void encodeFrame() override{

        }

        void endEncode() override{
            VTCompressionSessionInvalidate(compSession);
            CFRelease(compSession);
        }   

        void beginDecode() override{
            VTDecompressionSessionCreate(NULL,NULL,NULL,NULL,NULL,&decompSession);
        }

        void decodeFrame() override {

        }

        void endDecode() override{
            VTDecompressionSessionInvalidate(decompSession);
            CFRelease(compSession);
        }

        AVFAudioVideoProcessor(){
            
        };
        ~AVFAudioVideoProcessor(){

        };
    };

    AudioVideoProcessor * avProcessor = new AVFAudioVideoProcessor();
};