#include "../AudioVideoProcessor.h"

#include <libavcodec/avcodec.h>

namespace OmegaWTK::Media {
    class FFmpegAudioVideoProcessor : public AudioVideoProcessor {


        AVCodecContext *context = nullptr;
        AVCodec *currentCodec = nullptr;

    public:
        void beginEncode(BeginEncodeParams params) override{
            currentCodec = avcodec_find_encoder(AV_CODEC_ID_HEVC);
            context = avcodec_alloc_context3(currentCodec);
            avcodec_open2(context,currentCodec,NULL);
        }
    
        void encodeFrame(EncodeFrameParams params) override{
            auto frame = av_frame_alloc();
            avcodec_send_frame(context,frame);
            auto *result = av_packet_alloc();

            avcodec_receive_packet(context,result);

            av_packet_free(&result);
            av_frame_free(&frame);
        }

        void endEncode() override{
            avcodec_close(context);
            avcodec_free_context(&context);
        }

        void beginDecode(BeginDecodeParams params) override{
            currentCodec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
            context = avcodec_alloc_context3(currentCodec);
            avcodec_open2(context,currentCodec,NULL);
        }

        void decodeFrame(DecodeFrameParams params) override {
            auto packet = av_packet_alloc();
            avcodec_send_packet(context,packet);
            AVFrame *result = av_frame_alloc();

            avcodec_receive_frame(context,result);

            av_frame_free(&result);
            av_packet_free(&packet);
        }

        void endDecode() override{
            avcodec_close(context);
            avcodec_free_context(&context);
        }

        FFmpegAudioVideoProcessor(){
            
        };
        ~FFmpegAudioVideoProcessor(){

        };
    };

    Core::UniquePtr<AudioVideoProcessor> avProcessor;
};