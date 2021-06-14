
#import <AVFoundation/AVFoundation.h>
#include "omegaWTK/Media/VideoCapture.h"
#include "omegaWTK/Media/AudioCapture.h"

namespace OmegaWTK::Media {

    class AVFVideoCaptureSession : public VideoCaptureSession {

    };

    class AVFVideoCaptureDevice : public VideoDevice {
        AVCaptureDevice *device;
    public:
        AVFVideoCaptureDevice(AVCaptureDevice *device):device(device){

        }
        ~AVFVideoCaptureDevice(){
            [device release];
        };
        UniqueHandle<VideoCaptureSession> createCaptureSession(){
            
        };
    };

    Core::Vector<VideoDevice *> enumerateVideoDevices() {
        AVCaptureDeviceDiscoverySession *discovery = [AVCaptureDeviceDiscoverySession discoverySessionWithDeviceTypes:@[AVCaptureDeviceTypeBuiltInWideAngleCamera] mediaType:AVMediaTypeVideo position:AVCaptureDevicePositionFront];
        auto devices = discovery.devices;
        Core::Vector<VideoDevice *> outDevices;
        for (AVCaptureDevice *dev : devices) {
            outDevices.push_back(new AVFVideoCaptureDevice(dev));
        }
        return outDevices;
    };


};