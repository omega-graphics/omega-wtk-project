#include "omegaWTK/Media/Audio.h"
#include "omegaWTK/Media/Video.h"
#include "omegaWTK/Media/MediaPlaybackSession.h"

#import <AVFoundation/AVFoundation.h>
//#import <AudioToolbox/AudioToolbox.h>

#include <cassert>




namespace OmegaWTK::Media {


    struct AVFAudioCaptureDevice : public AudioCaptureDevice {
        __strong AVCaptureDevice *device;
        explicit AVFAudioCaptureDevice(AVCaptureDevice *dev):device(dev){

        };
        UniqueHandle<AudioCaptureSession> createCaptureSession() override {

        }
        ~AVFAudioCaptureDevice() = default;
    };

    OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> enumerateAudioCaptureDevices(){

        AVCaptureDeviceDiscoverySession *session = [AVCaptureDeviceDiscoverySession
                                                    discoverySessionWithDeviceTypes:
        @[AVCaptureDeviceTypeBuiltInMicrophone,AVCaptureDeviceTypeExternalUnknown] mediaType:AVMediaTypeAudio position:AVCaptureDevicePositionUnspecified];

        OmegaCommon::Vector<SharedHandle<AudioCaptureDevice>> devs;
        for(AVCaptureDevice *dev in session.devices){
            devs.emplace_back(new AVFAudioCaptureDevice(dev));
        }

        return devs;
    };

    struct  AudioPlaybackDevice {
        __strong AVAudioOutputNode *node;
        explicit AudioPlaybackDevice(AUAudioUnitV2Bridge * dev){
//            AVAudioSinkNode *sink = [[AVAudioSinkNode alloc] ini]
        };
        ~AudioPlaybackDevice() = default;
    };

    typedef AudioPlaybackDevice AVFAudioPlaybackDevice;

     OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> enumerateAudioPlaybackDevices(){
         AudioComponent in = NULL;
         AudioComponentDescription desc {};

         OmegaCommon::Vector<SharedHandle<AudioPlaybackDevice>> devices;

         while((in = AudioComponentFindNext(in,&desc)) != NULL){
             if(desc.componentType == kAudioUnitType_Generator) {
                 if(desc.componentSubType == kAudioUnitSubType_GenericOutput
                    || desc.componentSubType == kAudioUnitSubType_SystemOutput
                    || desc.componentSubType == kAudioUnitSubType_DefaultOutput) {
                     NSError *error;
                     AUAudioUnit *unit = [[AUAudioUnit alloc] initWithComponentDescription:desc error:&error];
                     AUAudioUnitV2Bridge *bridge = unit;
                     devices.emplace_back(new AudioPlaybackDevice(bridge));
                 }
             }
         };

         return devices;

     };


     struct AVFVideoDevice : public VideoDevice {
         __strong AVCaptureDevice *device;
         explicit AVFVideoDevice(AVCaptureDevice *dev):device(dev){

         };
         UniqueHandle<VideoCaptureSession> createCaptureSession() override {

         }
         ~AVFVideoDevice() = default;
     };

    OmegaCommon::Vector<SharedHandle<VideoDevice>> enumerateVideoDevices(){
        AVCaptureDeviceDiscoverySession *session = [AVCaptureDeviceDiscoverySession
                discoverySessionWithDeviceTypes:
                        @[AVCaptureDeviceTypeBuiltInWideAngleCamera,AVCaptureDeviceTypeExternalUnknown] mediaType:AVMediaTypeVideo position:AVCaptureDevicePositionUnspecified];

        OmegaCommon::Vector<SharedHandle<VideoDevice>> devs;
        for(AVCaptureDevice *dev in session.devices){
            devs.emplace_back(new AVFVideoDevice(dev));
        }

        return devs;
    }



    class AVFAudioPlaybackSession : public AudioPlaybackSession {
        AudioInputStream *in = nullptr;
        SharedHandle<AVFAudioPlaybackDevice> playbackDevice = nullptr;
    public:
        void setAudioPlaybackDevice(SharedHandle<AudioPlaybackDevice> &device) override {
            playbackDevice = std::dynamic_pointer_cast<AVFAudioPlaybackDevice>(device);
        }
        void setAudioSource(AudioInputStream &inputStream) override {
            in = &inputStream;
        }
        void start() override {
            assert(playbackDevice && "No playback device has been set. Exiting...");

        }
        void pause() override {

        }
        void reset() override {

        }
        ~AVFAudioPlaybackSession() = default;
    };

    SharedHandle<AudioPlaybackSession> AudioPlaybackSession::Create() {
        return SharedHandle<AudioPlaybackSession>(new AVFAudioPlaybackSession());
    }

}