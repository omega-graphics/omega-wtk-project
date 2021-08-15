

#include "../AudioVideoProcessor.h"

#include <mfidl.h>
#include <ShlObj.h>
#include <mfapi.h>

#pragma comment(lib,"mf.lib")

namespace OmegaWTK::Media {

    class WMFAudioVideoProcessor : public AudioVideoProcessor {
        IMFMediaSession *currentSession;
    public:

        void beginEncode() override{
            MFCreateMediaSession(NULL,&currentSession);
        }
    
        void encodeFrame() override{

        }

        void endEncode() override{
            Core::SafeRelease(&currentSession);
        }

        void beginDecode() override{
           MFCreateMediaSession(NULL,&currentSession);
        }

        void decodeFrame() override {
            
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

    AudioVideoProcessor *avProcessor = new WMFAudioVideoProcessor();


};