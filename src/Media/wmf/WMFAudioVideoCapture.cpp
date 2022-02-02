#include "omegaWTK/Media/MediaPlaybackSession.h"

#include "omegaWTK/Core/Core.h"

#include <mfidl.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <mfapi.h>
#include <mmdeviceapi.h>
#include <mfcaptureengine.h>

#include <memory>

#include <d3d11_4.h>
#include <d3d12video.h>

#pragma comment(lib,"mfplat.lib")
#pragma comment(lib,"mfuuid.lib")

namespace OmegaWTK::Media {

    void cpp_to_wstring(OmegaCommon::StrRef str,LPWSTR * output){
        UChar *temp = new UChar[str.size()];
        int32_t len;
        UErrorCode err;
        u_strFromUTF8(temp,str.size(),&len,str.data(),str.size(),&err);
        *output = (LPWSTR)temp;
    }

    IMFByteStream *createMFByteStreamMediaInputStream(MediaInputStream &inputStream){
        IMFByteStream *byteStream;
        if(inputStream.bufferOrFile){
            IStream *str = SHCreateMemStream((BYTE *)inputStream.buffer.data,inputStream.buffer.length);
            MFCreateMFByteStreamOnStream(str,&byteStream);
        }
        else {
            LPWSTR str = nullptr;
            cpp_to_wstring(inputStream.file,&str);
            MFCreateFile(MF_ACCESSMODE_READ,MF_OPENMODE_FAIL_IF_NOT_EXIST,MF_FILEFLAGS_NOBUFFERING,str,&byteStream);
        }
        return byteStream;
    }

    IMFByteStream *createMFByteStreamMediaOutputStream(MediaOutputStream &outputStream){
        IMFByteStream *byteStream;
        if(outputStream.bufferOrFile){
            IStream *str = SHCreateMemStream((BYTE *)outputStream.buffer.data,outputStream.buffer.length);
            MFCreateMFByteStreamOnStream(str,&byteStream);
        }
        else {
            LPWSTR str = nullptr;
            cpp_to_wstring(outputStream.file,&str);
            MFCreateFile(MF_ACCESSMODE_WRITE,MF_OPENMODE_DELETE_IF_EXIST,MF_FILEFLAGS_NOBUFFERING,str,&byteStream);
        }
        return byteStream;
    }


    /// AudioVideoProcessor Interface
    /// @{
    IMFActivate *AudioVideoProcessor_GetSoftwareEncodeTransform(UniqueHandle<AudioVideoProcessor> & processor);

    IMFActivate *AudioVideoProcessor_GetSoftwareDecodeTransform(UniqueHandle<AudioVideoProcessor> & processor);

    void AudioVideoProcessor_EncodeFrame(UniqueHandle<AudioVideoProcessor> & processor,IMFSample *sample,IMFSample **output);

    void AudioVideoProcessor_DecodeFrame(UniqueHandle<AudioVideoProcessor> & processor,IMFSample *sample,IMFSample **output);

    void AudioVideoProcessor_setDecodeCodec(UniqueHandle<AudioVideoProcessor> & processor,const GUID &from,const GUID &to);

    void AudioVideoProcessor_setEncodeCodec(UniqueHandle<AudioVideoProcessor> & processor,const GUID &from,const GUID &to);
    ///@}


    class WMFAudioVideoProcessorClient : public IMFTransform {
        unsigned refCount = 1;
        OmegaCommon::Map<DWORD,IMFStreamDescriptor *> outputStreams,inputStreams;
        OmegaCommon::Vector<IMFMediaType *> outputStreamMediaTypes,inputStreamMediaTypes;
        bool useHEVC;
    public:
        explicit WMFAudioVideoProcessorClient(bool encodeOrDecode,bool useHEVC):useHEVC(useHEVC){
            IMFMediaType *type;
            MFCreateMediaType(&type);
            type->SetGUID(MF_MT_MAJOR_TYPE,MFMediaType_Video);
            if(useHEVC) {
                type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_HEVC);
            }
            else {
                type->SetGUID(MF_MT_SUBTYPE,MFVideoFormat_H264);
            }
            if(encodeOrDecode){
                outputStreamMediaTypes.push_back(type);
            }
            else {
                inputStreamMediaTypes.push_back(type);
            }

            /// Encode Input / Decode Output

            type->SetGUID(MF_MT_SUBTYPE,MFVideoFormat_RGB32);
            if(encodeOrDecode){
                inputStreamMediaTypes.push_back(type);
            }
            else {
                outputStreamMediaTypes.push_back(type);
            }
        }
        ULONG STDMETHODCALLTYPE AddRef() override{
            return InterlockedIncrement(&refCount);
        }
        HRESULT STDMETHODCALLTYPE QueryInterface(const IID &iid,void **ppData) override{

        }
        ULONG STDMETHODCALLTYPE Release() override {
            ULONG newCount = InterlockedDecrement(&refCount);
            if(newCount == 0){
                delete this;
            }
            return newCount;
        }
        HRESULT AddInputStreams(DWORD cStreams, DWORD *adwStreamIDs) override {
            for(;cStreams > 0;cStreams--){
                inputStreams.insert(std::make_pair(*adwStreamIDs,nullptr));
                ++adwStreamIDs;
            }
            return S_OK;
        }
        HRESULT GetInputAvailableType(DWORD dwInputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType) override {
            *ppType = inputStreamMediaTypes[dwTypeIndex];
            return S_OK;
        }
        HRESULT GetOutputAvailableType(DWORD dwOutputStreamID, DWORD dwTypeIndex, IMFMediaType **ppType) override {
            *ppType = outputStreamMediaTypes[dwTypeIndex];
            return S_OK;
        }
        HRESULT SetOutputType(DWORD dwOutputStreamID, IMFMediaType *pType, DWORD dwFlags) override {
            auto & stream = outputStreams[dwOutputStreamID];
            if(stream != nullptr){
                stream->Release();
            }
            MFCreateStreamDescriptor(dwOutputStreamID,1,&pType,&stream);
            return S_OK;
        }
        HRESULT SetInputType(DWORD dwInputStreamID, IMFMediaType *pType, DWORD dwFlags) override {
            auto & stream = inputStreams[dwInputStreamID];
            if(stream != nullptr){
                stream->Release();
            }
            MFCreateStreamDescriptor(dwInputStreamID,1,&pType,&stream);
            return S_OK;
        }
        HRESULT ProcessInput(DWORD dwInputStreamID, IMFSample *pSample, DWORD dwFlags) override {

        }
        HRESULT ProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER *pOutputSamples, DWORD *pdwStatus) override {
            return S_OK;
        }
        ~WMFAudioVideoProcessorClient() = default;
    };





    class WMFVideoSampleGrabber : public IMFSampleGrabberSinkCallback {
        VideoFrameSink *visualSink;
        unsigned refCount = 1;
        Core::Rect frameRect;
        IMFPresentationClock *clock;
    public:
        explicit WMFVideoSampleGrabber(VideoFrameSink * sink,const Core::Rect & frameRect):
        visualSink(sink),
        frameRect(frameRect){

        }
        ULONG STDMETHODCALLTYPE AddRef() override{
            return InterlockedIncrement(&refCount);
        }
        HRESULT STDMETHODCALLTYPE QueryInterface(const IID &iid,void **ppData) override{
            if(iid == IID_IUnknown || iid == IID_IMFSampleGrabberSinkCallback){
                *ppData = this;
                return S_OK;
            }
            else {
                *ppData = nullptr;
                return E_NOINTERFACE;
            }
        }
        ULONG STDMETHODCALLTYPE Release() override {
            ULONG newCount = InterlockedDecrement(&refCount);
            if(newCount == 0){
                delete this;
            }
            return newCount;
        }
        HRESULT STDMETHODCALLTYPE  OnClockStart(
                /* [in] */ MFTIME hnsSystemTime,
                /* [in] */ LONGLONG llClockStartOffset) override {
            return S_OK;
        };
        HRESULT STDMETHODCALLTYPE OnClockStop(
                /* [in] */ MFTIME hnsSystemTime) override {
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE OnClockPause(
                /* [in] */ MFTIME hnsSystemTime) override {
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE OnClockRestart(
                /* [in] */ MFTIME hnsSystemTime) override {
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE OnClockSetRate(
                /* [in] */ MFTIME hnsSystemTime,
                /* [in] */ float flRate) override {
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE OnProcessSample(const GUID &guidMajorMediaType, DWORD dwSampleFlags, LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE *pSampleBuffer, DWORD dwSampleSize) override {
            auto frame = std::make_shared<VideoFrame>();

            MFTIME time;
            clock->GetTime(&time);

            frame->decodeFinishTime = TimePoint(std::chrono::nanoseconds(time * 100));
            frame->presentTime = TimePoint(std::chrono::nanoseconds(llSampleTime * 100));
            frame->videoFrame.data = (ImgByte *)pSampleBuffer;
            frame->videoFrame.header.height = (uint32_t)frameRect.h;
            frame->videoFrame.header.width = (uint32_t)frameRect.w;
            frame->videoFrame.header.color_format = BitmapImage::ColorFormat::RGBA;
            frame->videoFrame.header.bitDepth = 8;
            frame->videoFrame.header.channels = 4;
            frame->videoFrame.header.alpha_format = BitmapImage::AlphaFormat::Straight;
            visualSink->pushFrame(frame);

            if(time > llSampleTime){
                visualSink->flush();
            }
            else {
                auto millis = std::chrono::milliseconds((llSampleTime - time)/10);
                std::this_thread::sleep_for(millis);
                visualSink->presentCurrentFrame();
            }
        }
        HRESULT STDMETHODCALLTYPE OnSetPresentationClock(IMFPresentationClock *pPresentationClock) override {
            pPresentationClock->AddRef();
            clock = pPresentationClock;
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE OnShutdown() override {
            return S_OK;
        }
    };


    class PlaybackDispatchQueue {
        DWORD workQueue;
    public:
        explicit PlaybackDispatchQueue():workQueue(0){
            MFAllocateWorkQueue(&workQueue);
        }
        ~PlaybackDispatchQueue(){
            MFUnlockWorkQueue(workQueue);
        }
    };

    SharedHandle<PlaybackDispatchQueue> createPlaybackDispatchQueue(){
        return std::make_shared<PlaybackDispatchQueue>();
    }

    struct AudioPlaybackDevice {
        IMFMediaSink *sink;
        IMMDevice *device;
    };

   struct WMFAudioCaptureDevice : public AudioCaptureDevice {
        IMFActivate *activate;
   public:
       explicit WMFAudioCaptureDevice(IMFActivate *activate):activate(activate){

       }
       UniqueHandle<AudioCaptureSession> createCaptureSession() override;
    };

    OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> enumerateAudioCaptureDevices(){
        OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> devs;
        IMFAttributes *attrs;
        MFCreateAttributes(&attrs,0);
        attrs->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID);
        IMFActivate ** activates;
        UINT32 len;
        MFEnumDeviceSources(attrs,&activates,&len);
        for(;len > 0;len--){
            IMFActivate *activate = *activates;
            devs.emplace_back(new WMFAudioCaptureDevice(activate));
            ++activates;
        }
        Core::SafeRelease(&attrs);
        return devs;
    }

    struct WMFVideoDevice : public VideoDevice {
        IMFActivate *activate;
    public:
        explicit WMFVideoDevice(IMFActivate *activate):activate(activate){

        }
        UniqueHandle<VideoCaptureSession> createCaptureSession(SharedHandle<AudioCaptureDevice> &audioCaptureDevice) override;
    };

    OmegaCommon::Vector<SharedHandle<VideoDevice>> enumerateVideoDevices(){
        OmegaCommon::Vector<SharedHandle<VideoDevice>> devs;
        IMFAttributes *attrs;
        MFCreateAttributes(&attrs,0);
        attrs->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
        IMFActivate ** activates;
        UINT32 len;
        MFEnumDeviceSources(attrs,&activates,&len);
        for(;len > 0;len--){
            IMFActivate *activate = *activates;
            devs.emplace_back(new WMFVideoDevice(activate));
            ++activates;
        }
        Core::SafeRelease(&attrs);
        return devs;
    }

    Microsoft::WRL::ComPtr<IMFCaptureEngineClassFactory> captureEngineFactory = nullptr;

    class WMFAudioCaptureSession : public AudioCaptureSession {
        IMFCaptureEngine *engine;
    public:
        explicit WMFAudioCaptureSession(WMFAudioCaptureDevice * device){
            if(captureEngineFactory.Get() == nullptr){
                CoCreateInstance(CLSID_MFCaptureEngineClassFactory,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&captureEngineFactory));
            }
            captureEngineFactory->CreateInstance(CLSID_MFCaptureEngine,IID_PPV_ARGS(&engine));
            IMFAttributes *attrs;
            MFCreateAttributes(&attrs,0);
            attrs->SetUINT32(MF_CAPTURE_ENGINE_USE_AUDIO_DEVICE_ONLY,TRUE);
            engine->Initialize(nullptr,attrs,device->activate,nullptr);
        }
        void setAudioOutputStream(MediaOutputStream &outputStream) override {
            IMFCaptureRecordSink *sink;
            engine->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_RECORD, reinterpret_cast<IMFCaptureSink **>(&sink));
            if(outputStream.bufferOrFile){
                sink->SetOutputByteStream(createMFByteStreamMediaOutputStream(outputStream),MFTranscodeContainerType_MP3);
            }
            else {
                LPWSTR wstr;
                cpp_to_wstring(outputStream.file,&wstr);
                sink->SetOutputFileName(wstr);
            }
        }
        void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> &device) override {
            IMFCapturePreviewSink *sink;
            engine->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_PREVIEW, reinterpret_cast<IMFCaptureSink **>(&sink));
            sink->SetCustomSink(device->sink);
        }
        void startPreview() override {
            engine->StartPreview();
        }
        void startRecord() override {
            engine->StartRecord();
        }
        void stopRecord() override {
            engine->StopRecord(TRUE,TRUE);
        }
        void stopPreview() override {
            engine->StopPreview();
        }
    };

    UniqueHandle<AudioCaptureSession> WMFAudioCaptureDevice::createCaptureSession() {
        return (UniqueHandle<AudioCaptureSession>)new WMFAudioCaptureSession(this);
    }

    class WMFVideoCaptureSampleSink : public IMFCaptureEngineOnSampleCallback {
        unsigned refCount = 1;
        VideoFrameSink *sink;
    public:
        explicit WMFVideoCaptureSampleSink(VideoFrameSink *sink):sink(sink){

        }
        ULONG AddRef() override{
            return InterlockedIncrement(&refCount);
        }
        HRESULT QueryInterface(const GUID &iid,void **pData) override{
            return S_OK;
        }
        HRESULT OnSample(IMFSample *pSample) override {
            return S_OK;
        }
        ULONG Release() override{
            ULONG newCount = InterlockedDecrement(&refCount);
            if(newCount == 0){
                delete this;
            }
            return newCount;
        }
    };

    class WMFVideoCaptureSession : public VideoCaptureSession {
        IMFCaptureEngine *engine;
        Core::UniqueComPtr<WMFVideoCaptureSampleSink> videoPreviewSink;
    public:
        explicit WMFVideoCaptureSession(WMFVideoDevice * videoDevice,WMFAudioCaptureDevice * audioDevice){
            if(captureEngineFactory.Get() == nullptr){
                CoCreateInstance(CLSID_MFCaptureEngineClassFactory,NULL,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&captureEngineFactory));
            }
            captureEngineFactory->CreateInstance(CLSID_MFCaptureEngine,IID_PPV_ARGS(&engine));
            IMFAttributes *attrs;
            MFCreateAttributes(&attrs,0);
            engine->Initialize(nullptr,attrs,audioDevice->activate,videoDevice->activate);
        }
        void setAudioPlaybackDeviceForPreview(SharedHandle<AudioPlaybackDevice> &device) override {
            IMFCapturePreviewSink *sink;
            engine->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_PREVIEW,reinterpret_cast<IMFCaptureSink **>(&sink));
            sink->SetCustomSink(device->sink);
        }
        void setVideoFrameSinkForPreview(VideoFrameSink &frameSink) override {

            auto ptr = new WMFVideoCaptureSampleSink(&frameSink);
            if(videoPreviewSink.get() != nullptr){
                videoPreviewSink->Release();
            }
            videoPreviewSink = ptr;
            IMFCapturePreviewSink *sink;
            engine->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_PREVIEW,reinterpret_cast<IMFCaptureSink **>(&sink));
            sink->SetSampleCallback(0,videoPreviewSink.get());
        }
        void setVideoOutputStream(MediaOutputStream &outputStream) override {
            IMFCaptureRecordSink *sink;
            engine->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_RECORD,reinterpret_cast<IMFCaptureSink **>(&sink));
            if(outputStream.bufferOrFile){
                sink->SetOutputByteStream(createMFByteStreamMediaOutputStream(outputStream),MFTranscodeContainerType_MPEG4);
            }
            else {
                LPWSTR wstr;
                cpp_to_wstring(outputStream.file,&wstr);
                sink->SetOutputFileName(wstr);
            }
        }
        void startPreview() override {
            engine->StartPreview();
        }
        void startRecord() override {
            engine->StartRecord();
        }
        void stopRecord() override {
            engine->StopRecord(TRUE,TRUE);
        }
        void stopPreview() override {
            engine->StopPreview();
        }
    };

    UniqueHandle<VideoCaptureSession>
    WMFVideoDevice::createCaptureSession(SharedHandle<AudioCaptureDevice> &audioCaptureDevice) {
        return (UniqueHandle<VideoCaptureSession>)new WMFVideoCaptureSession(this,(WMFAudioCaptureDevice *)audioCaptureDevice.get());
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
        IMFSourceResolver *sourceResolver;
        PROPVARIANT p;
        IMFMediaSource *mediaSource;
    public:
        explicit WMFAudioPlaybackSession(UniqueHandle<AudioVideoProcessor> & processor) : AudioPlaybackSession(processor){
            PropVariantInit(&p);
            IMFAttributes *attrs;
            MFCreateAttributes(&attrs,0);
            MFCreateMediaSession(attrs,&session);
            MFCreateTopology(&topology);
            MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE,&outputNode);
            topology->AddNode(outputNode);
            MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE,&sourceNode);
            topology->AddNode(sourceNode);
            MFCreateSourceResolver(&sourceResolver);
            sourceNode->ConnectOutput(0,outputNode,0);
            session->SetTopology(0,topology);
            Core::SafeRelease(&attrs);
        }
        void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> &device) override {
            outputNode->SetObject(device->sink);
        }
        void setAudioSource(MediaInputStream &inputStream) override {
            MF_OBJECT_TYPE t;
            IUnknown *obj;
            sourceResolver->CreateObjectFromByteStream(createMFByteStreamMediaInputStream(inputStream),NULL,MF_RESOLUTION_MEDIASOURCE,NULL,&t,&obj);
            obj->QueryInterface(IID_PPV_ARGS(&mediaSource));
            sourceNode->SetUnknown(MF_TOPONODE_SOURCE,mediaSource);
            IMFPresentationDescriptor *desc;
            mediaSource->CreatePresentationDescriptor(&desc);
            sourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR,desc);
            IMFStreamDescriptor *streamDesc;
            BOOL selected;
            desc->GetStreamDescriptorByIndex(0,&selected,&streamDesc);
            sourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR,streamDesc);
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

    SharedHandle<AudioPlaybackSession> AudioPlaybackSession::Create(UniqueHandle<AudioVideoProcessor> & processor,SharedHandle<PlaybackDispatchQueue> & queue) {
        return (SharedHandle<AudioPlaybackSession>)new WMFAudioPlaybackSession(processor);
    }

    class WMFVideoPlaybackSession : public VideoPlaybackSession {
        IMFMediaSession *session;
        IMFTopology *topology;
        IMFTopologyNode *sourceNode,*videoOutputNode,*audioOutputNode;
        SharedHandle<WMFVideoSampleGrabber> videoSink;
        IMFActivate *videoSampleGrabber;
        IMFSourceResolver *sourceResolver;
        PROPVARIANT p;
        IMFMediaSource *mediaSource;
        Core::Rect frameRect;
    public:
        explicit WMFVideoPlaybackSession(UniqueHandle<AudioVideoProcessor> & processor) : VideoPlaybackSession(processor){
            IMFAttributes *attrs;
            MFCreateAttributes(&attrs,0);
            MFCreateMediaSession(attrs,&session);
            MFCreateTopology(&topology);
            MFCreateSourceResolver(&sourceResolver);
            PropVariantInit(&p);

            MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE,&videoOutputNode);


            Core::SafeRelease(&attrs);
        }
        void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> &device) override {
            audioOutputNode->SetObject(device->sink);
        }
        void setVideoFrameSink(VideoFrameSink &sink) override {
            videoSink = std::make_shared<WMFVideoSampleGrabber>(&sink,frameRect);
            IMFPresentationDescriptor *presentDesc;
            mediaSource->CreatePresentationDescriptor(&presentDesc);
            IMFMediaType *type;
            MFCreateMediaType(&type);
            type->SetGUID(MF_MT_MAJOR_TYPE,MFMediaType_Video);
            type->SetGUID(MF_MT_SUBTYPE,MFVideoFormat_v410);
            MFCreateSampleGrabberSinkActivate(type,videoSink.get(),&videoSampleGrabber);
            videoOutputNode->SetObject(videoSampleGrabber);
        }
        void setVideoSource(MediaInputStream &inputStream) override {
            MF_OBJECT_TYPE t;
            IUnknown *obj;
            sourceResolver->CreateObjectFromByteStream(createMFByteStreamMediaInputStream(inputStream),NULL,0,NULL,&t,&obj);
            obj->QueryInterface(IID_PPV_ARGS(&mediaSource));
            sourceNode->SetUnknown(MF_TOPONODE_SOURCE,mediaSource);
            IMFPresentationDescriptor *presentDesc;
            mediaSource->CreatePresentationDescriptor(&presentDesc);
            sourceNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR,presentDesc);
            IMFStreamDescriptor *streamDesc;
            BOOL selected;
            presentDesc->GetStreamDescriptorByIndex(0,&selected,&streamDesc);
            sourceNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR,streamDesc);
            IMFMediaTypeHandler *handler;
            streamDesc->GetMediaTypeHandler(&handler);
            IMFMediaType *type;
            handler->GetCurrentMediaType(&type);
            UINT32 w,h;
            MFGetAttributeSize(type,MF_MT_FRAME_SIZE,&w,&h);
            frameRect = Core::Rect {Core::Position {0,0},(float)w,(float)h};
        }
        void start() override {

        }
        void pause() override {

        }
        void reset() override {

        }
    };
}