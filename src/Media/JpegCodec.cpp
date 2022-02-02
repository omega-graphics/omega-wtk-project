#include "ImgCodecPriv.h"

#include <turbojpeg.h>

namespace OmegaWTK::Media {

    class JPEGCodec : public ImgCodec {
        typedef unsigned char tjByte;
        bool load_jpeg_from_file(){
            auto decomp = tjInitDecompress();
            in.seekg(0,in.end);
            size_t len = in.tellg();
            in.seekg(0,in.beg);
            auto dataBuf = new tjByte[len];
            int w,h;
            int samp;
            int colorspace;
            tjDecompressHeader3(decomp,dataBuf,len,&w,&h,&samp,&colorspace);
            storage->data = new tjByte[w * h * 4];
            tjDecompress2(decomp,dataBuf,len,(unsigned char *)storage->data,w,w * 4,h,TJPF_RGBA,TJFLAG_BOTTOMUP | TJFLAG_ACCURATEDCT);
            
            tjDestroy(decomp);
            delete [] dataBuf;
            return true;
        };
    public:
        void readToStorage(){
            if(!load_jpeg_from_file()){
                storage->data = nullptr;
                // storage->header = nullptr;
                // storage->profile = nullptr;
            };
        };
        JPEGCodec(Core::IStream & stream,BitmapImage *res):ImgCodec(stream,res){};
    };

    Core::UniquePtr<ImgCodec> getJpegCodec(Core::IStream &in,BitmapImage *img){
        return std::make_unique<JPEGCodec>(in,img);
    };

}