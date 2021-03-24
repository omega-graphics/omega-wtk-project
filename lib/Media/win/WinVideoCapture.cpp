#include "omegaWTK/Media/VideoCapture.h"

#include <mfidl.h>
#include <mfcaptureengine.h>

#pragma comment(lib,"mf.lib")

namespace OmegaWTK::Media {
    class WinVideoCaptureSession : public VideoCaptureSession {
        
    };

    class WinVideoDevice : public VideoDevice {
        UniqueHandle<VideoCaptureSession> createCaptureSession();
    };

    Core::Vector<VideoDevice *> enumerateVideoDevices(){

    };
};