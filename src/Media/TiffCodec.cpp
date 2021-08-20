#include "ImgCodecPriv.h"

#include <cstdio>
#include <cstdlib>

#include <tiffio.h>

namespace OmegaWTK::Media {

    

    class TiffIStream {
        std::istream * in = nullptr;
        thandle_t self;

        size_t strSize;

        TIFF *tiff;

        static tmsize_t readProc(thandle_t s,void * data,tmsize_t size){
            auto *str = (TiffIStream *)s;
            size_t dataLeft = str->strSize - str->in->tellg();
            auto amountToRead = dataLeft > size ? size : dataLeft;
            str->in->read((char *)data,amountToRead);
            return str->in->gcount();
        }

        static tmsize_t writeProc(thandle_t s,void * data,tmsize_t size){
             auto *str = (TiffIStream *)s;
             return 0;
        }

        static toff_t seekProc(thandle_t s,toff_t off,int whence){
             auto *str = (TiffIStream *)s;
             std::ios::seekdir dir;
            switch (whence) {
                case SEEK_SET : {
                    dir = std::ios::beg;
                }
                case SEEK_CUR : {
                    dir = std::ios::cur;
                }
                case SEEK_END : {
                    dir = std::ios::end;
                }
                default : {
                    dir = std::ios::beg;
                    break;
                }
            }
            str->in->seekg(off,dir);
            return str->in->tellg();
        }

        static toff_t sizeProc(thandle_t s){
             auto *str = (TiffIStream *)s;
             str->in->seekg(0,std::ios::end);
             auto size = str->in->tellg();
             str->in->seekg(0,std::ios::beg);
             return toff_t(size);
        }

        static int closeProc(thandle_t s){
            auto *str = (TiffIStream *)s;
            str->in = nullptr;
            return -1;
        }

    public:
        TiffIStream():self((thandle_t)this){

        };
        TIFF * open(std::istream * in){
            this->in = in;
            in->seekg(0,std::ios::end);
            strSize = in->tellg();
            in->seekg(0,std::ios::beg);
            
            return TIFFClientOpen("TiffIStream","rb",self,readProc,writeProc,seekProc,closeProc,sizeProc,nullptr,nullptr);
        };
       
        
    };

    class TiffCodec : public ImgCodec {
        bool load_tiff_from_file(){
            TiffIStream tiffStream;
            TIFF *tiff = tiffStream.open(&in);

            uint32_t width,height,bitDepth,channelCount,compression;
            
            ImgHeader header;

            TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH,&width);
            TIFFGetField(tiff,TIFFTAG_IMAGELENGTH,&height);
            TIFFGetField(tiff,TIFFTAG_BITSPERSAMPLE,&bitDepth);
            TIFFGetField(tiff,TIFFTAG_SAMPLESPERPIXEL,&channelCount);
            TIFFGetField(tiff,TIFFTAG_COMPRESSION,&compression);
            
            header.color_format = BitmapImage::ColorFormat::RGBA;
            header.bitDepth = bitDepth;
            header.channels = channelCount;
            header.width = width;
            header.height = height;
            
            bool rc = false;

            size_t pixelCount = width * height;
            uint32_t * buffer = (uint32_t *)_TIFFmalloc(pixelCount * sizeof(uint32_t));
            if(buffer != nullptr){
                if(TIFFReadRGBAImage(tiff,width,height,buffer,0)){
                    storage->data = (ImgByte *)buffer;
                    storage->header =std::make_unique<ImgHeader>(std::move(header));
                    storage->profile = nullptr;
                    rc = true;
                };
    //            _TIFFfree(buffer);
            };
            TIFFClose(tiff);
            return rc;
        };
    public:
        void readToStorage(){
            if(!load_tiff_from_file()){
                storage->data = nullptr;
                storage->header = nullptr;
                storage->profile = nullptr;
            };
        };
        TiffCodec(Core::IStream & stream,BitmapImage *res):ImgCodec(stream,res){};
    };

    Core::UniquePtr<ImgCodec> getTiffCodec(Core::IStream &in,BitmapImage *img){
        return std::make_unique<TiffCodec>(in,img);
    }

}