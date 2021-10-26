

#include "../AudioVideoProcessor.h"

#include <mfidl.h>
#include <ShlObj.h>
#include <mfapi.h>

#include <memory>

#include <d3d11_4.h>
#include <d3d12video.h>

#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mfuuid.lib")

namespace OmegaWTK::Media {

    class WMFAudioVideoProcessor : public AudioVideoProcessor {
        IMFTransform * transform;
        DWORD inputStreamID;
        DWORD outputStreamID;
        ID3D12VideoDevice2 *device;
    public:

        void beginEncode(BeginEncodeParams params) override{

            MFT_REGISTER_TYPE_INFO  t = {0};
            t.guidMajorType = MFMediaType_Video;
            t.guidSubtype = MFVideoFormat_HEVC;
            CLSID *clsid;
            UINT32 count;
            MFTEnum(MFT_CATEGORY_VIDEO_ENCODER,0,NULL,&t,NULL,&clsid,&count);
            CoCreateInstance(clsid[0],NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&transform));
            CoTaskMemFree(clsid);
            transform->GetStreamIDs(1,&inputStreamID,1,&outputStreamID);
        }
    
        void encodeFrame(EncodeFrameParams params) override {
            IMFSample *sample;
            CoCreateInstance(IID_IMFSample,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&sample));
            transform->ProcessInput(inputStreamID,sample,NULL);
            MFT_OUTPUT_DATA_BUFFER outputDataBuffer;
            DWORD status;
            transform->ProcessOutput(0,1,&outputDataBuffer,&status);
        }

        void endEncode() override{
            Core::SafeRelease(&transform);
        }

        void beginDecode(BeginDecodeParams params) override{
            D3D12_VIDEO_DECODER_DESC desc {};
            desc.Configuration.BitstreamEncryption = D3D12_BITSTREAM_ENCRYPTION_TYPE_NONE;
            desc.Configuration.DecodeProfile = D3D12_VIDEO_DECODE_PROFILE_HEVC_MAIN;
            desc.Configuration.InterlaceType = D3D12_VIDEO_FRAME_CODED_INTERLACE_TYPE_NONE;
            ID3D12VideoDecoder *decoder;
            device->CreateVideoDecoder(&desc,IID_PPV_ARGS(&decoder));
        }

        void decodeFrame(DecodeFrameParams params) override {
            
        }

        void endDecode() override{
            Core::SafeRelease(&transform);
        }
        
        explicit WMFAudioVideoProcessor():
        transform(nullptr),
        inputStreamID(0),
        outputStreamID(0){
            MFStartup(MF_VERSION);
        };
        ~WMFAudioVideoProcessor() override{
            MFShutdown();
        };
    };

    void AudioVideoProcessor::Initialize(){
        avProcessor = std::make_unique<WMFAudioVideoProcessor>();
    }

    void AudioVideoProcessor::Cleanup(){
        avProcessor.reset();
    }

    UniqueHandle<AudioVideoProcessor> avProcessor;


};