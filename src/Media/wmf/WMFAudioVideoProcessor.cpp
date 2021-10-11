

#include "../AudioVideoProcessor.h"

#include <mfidl.h>
#include <ShlObj.h>
#include <mfapi.h>

#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mf.lib")

namespace OmegaWTK::Media {

    class WMFAudioVideoProcessor : public AudioVideoProcessor {
        IMFMediaSession *currentSession;
    public:

        void beginEncode(BeginEncodeParams params) override{
            MFCreateMediaSession(NULL,&currentSession);
        }
    
        void encodeFrame(EncodeFrameParams params) override{

        }

        void endEncode() override{
            Core::SafeRelease(&currentSession);
        }

        void beginDecode(BeginDecodeParams params) override{
           MFCreateMediaSession(NULL,&currentSession);
        }

        void decodeFrame(DecodeFrameParams params) override {
            
        }

        void endDecode() override{
            Core::SafeRelease(&currentSession);
        }
        
        WMFAudioVideoProcessor(){
            MFStartup(MF_VERSION);
        };
        ~WMFAudioVideoProcessor(){
            MFShutdown();
        };
    };

    void AudioVideoProcessor::Initialize(){
        avProcessor.reset(new WMFAudioVideoProcessor());
    }

    void AudioVideoProcessor::Cleanup(){
        avProcessor.reset();
    }

    UniqueHandle<AudioVideoProcessor> avProcessor;


};