#include "omegaWTK/Media/MediaPlaybackSession.h"

#include <mfidl.h>
#include <ShlObj.h>
#include <mfapi.h>
#include <mmdeviceapi.h>

#include <memory>

#include <d3d11_4.h>
#include <d3d12video.h>

#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mfuuid.lib")

namespace OmegaWTK::Media {

    struct AudioPlaybackDevice {
        IMFMediaSink *sink;
        IMMDevice *device;
    };

   class WMFAudioCaptureDevice : public AudioCaptureDevice {
        IMFActivate *activate;
   public:
    };

    OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> enumerateAudioCaptureDevices(){
        IMFAttributes *attrs;
        MFCreateAttributes(&attrs,0);
        attrs->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID);
        IMFActivate ** activates;
        UINT32 len;
        MFEnumDeviceSources(attrs,&activates,&len);
        Core::SafeRelease(&attrs);
    }

    OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> enumerateAudioPlaybackDevices(){
        IMFAttributes *attrs;
        MFCreateAttributes(&attrs,0);
        const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
        const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
        IMMDeviceEnumerator *pEnumerator;
        HRESULT hr = CoCreateInstance(
                CLSID_MMDeviceEnumerator, NULL,
                CLSCTX_INPROC_SERVER, IID_IMMDeviceEnumerator,
                (void**)&pEnumerator);

        OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> res;

        IMMDeviceCollection *collection;
        pEnumerator->EnumAudioEndpoints(EDataFlow::eRender,DEVICE_STATE_ACTIVE | DEVICE_STATE_DISABLED,&collection);
        UINT count;
        collection->GetCount(&count);
        for(unsigned i = 0;i < count;i++){
            IMMDevice *device;
            IMFMediaSink *renderer;
            collection->Item(i,&device);
            LPWSTR id;
            device->GetId(&id);
            attrs->SetString(MF_AUDIO_RENDERER_ATTRIBUTE_ENDPOINT_ID,id);
            MFCreateAudioRenderer(attrs,&renderer);
            auto pb_dev = std::make_shared<AudioPlaybackDevice>();
            pb_dev->device = device;
            pb_dev->sink = renderer;
            res.push_back(pb_dev);
        }
        Core::SafeRelease(&attrs);
        return res;
    }

    class WMFAudioPlaybackSession : public AudioPlaybackSession {
        IMFMediaSession *session;
        IMFTopology *topology;
        IMFTopologyNode *sourceNode,*outputNode;
        PROPVARIANT p;
    public:
        explicit WMFAudioPlaybackSession(){
            PropVariantInit(&p);
            IMFAttributes *attrs;
            CoCreateInstance(IID_IMFAttributes,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&attrs));
            MFCreateMediaSession(attrs,&session);
            MFCreateTopology(&topology);
            Core::SafeRelease(&attrs);
        }
        void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> &device) override {
            IMFStreamSink *streamSink;
            device->sink->GetStreamSinkById(0,&streamSink);
            IMFTopologyNode *node;
            MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE,&node);
            node->SetObject(streamSink);
            topology->AddNode(node);
            outputNode = node;
        }
        void setAudioSource(AudioInputStream &inputStream) override {
            IMFMediaSource *source;
            IMFAsyncResult *res;
            IMFSourceResolver *resolver;
            MFCreateSourceResolver(&resolver);
            resolver->BeginCreateObjectFromURL(NULL,0,nullptr,nullptr,nullptr,nullptr);
            auto t = MF_OBJECT_MEDIASOURCE;
            resolver->EndCreateObjectFromByteStream(res, &t, reinterpret_cast<IUnknown **>(&source));
            IMFTopologyNode *node;
            MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE,&node);
            node->SetUnknown(MF_TOPONODE_SOURCE,source);
            topology->AddNode(node);
            node->ConnectOutput(0,outputNode,0);
            sourceNode = node;
        }
        void start() override {
            session->Start(nullptr,&p);
        }
        void pause() override {
            session->Stop();
        }
        void reset() override {
            PropVariantClear(&p);
            session->Shutdown();
        }
        ~WMFAudioPlaybackSession() override {
            session->Shutdown();
            Core::SafeRelease(&session);
        }
    };

    SharedHandle<AudioPlaybackSession> AudioPlaybackSession::Create() {
        return (SharedHandle<AudioPlaybackSession>)new WMFAudioPlaybackSession();
    }

    class WMFVideoPlaybackSession : public VideoPlaybackSession {

    };
}