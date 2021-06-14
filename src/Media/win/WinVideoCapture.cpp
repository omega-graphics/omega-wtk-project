#include "omegaWTK/Media/VideoCapture.h"

#include <mfidl.h>
#include <mfapi.h>
#include <mfcaptureengine.h>

#pragma comment(lib,"mf.lib")

namespace OmegaWTK::Media {
    class WinVideoCaptureSession : public VideoCaptureSession {
        
    };

    class WinVideoDevice : public VideoDevice {
        UniqueHandle<VideoCaptureSession> createCaptureSession();
    };

    Core::Vector<VideoDevice *> enumerateVideoDevices(){
        IMFMediaSource **sources = nullptr;
       IMFAttributes *attrs = nullptr;
       IMFActivate **devices = nullptr;
      HRESULT hr = MFCreateAttributes(&attrs,1);
      if(FAILED(hr)){

      };
      hr = attrs->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
      if(FAILED(hr)){

      };
      UINT32 count;
      hr = MFEnumDeviceSources(attrs,&devices, &count);
    };
};