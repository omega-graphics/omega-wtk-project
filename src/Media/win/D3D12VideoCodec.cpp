#include "omegaWTK/Media/VideoCodec.h"

#include <d3d12video.h>
#include <mfidl.h>
#include <atlbase.h>

#include "../VideoCodec.cpp"

namespace OmegaWTK::Media {

    class D3D12VideoCodec {
        Core::UniqueComPtr<ID3D12VideoDevice2> device;
        ID3D12Device8 *ptr;
    public:
        static Core::UniquePtr<D3D12VideoCodec> instance;
        D3D12VideoCodec(ID3D12Device8 *_ptr):ptr(_ptr){
            HRESULT hr;
            hr = ptr->QueryInterface(&device);
            if(FAILED(hr)){
                exit(1);
            };
            D3D12_FEATURE_DATA_VIDEO_DECODE_SUPPORT support;
            hr = device->CheckFeatureSupport(D3D12_FEATURE_VIDEO_DECODE_SUPPORT,&support,sizeof(support));
            if(FAILED(hr)){
                exit(1);
            };
            if(support.DecodeTier == D3D12_VIDEO_DECODE_TIER_NOT_SUPPORTED){
                exit(1);
            };
            
        };
    };

    void create_video_codec(void *gpu_device_ptr){
        D3D12VideoCodec::instance = std::make_unique<D3D12VideoCodec>((ID3D12Device8 *)gpu_device_ptr);
    };

    void destroy_video_codec(){
       D3D12VideoCodec::instance.reset();
    };

};