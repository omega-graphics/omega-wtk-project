

#include "../AudioVideoProcessor.h"

#include <mfidl.h>
#include <ShlObj.h>
#include <mfapi.h>

#pragma comment(lib,"mf.lib")

namespace OmegaWTK::Media {

    class WMFAudioVideoProcessor : public AudioVideoProcessor {
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
        
        WMFAudioVideoProcessor(){
            MFStartup(MF_VERSION);
        };
        ~WMFAudioVideoProcessor(){
            MFShutdown();
        };
    };

    AudioVideoProcessor *avProcessor = new WMFAudioVideoProcessor();


};