#include "omegaWTK/Media/ImgCodec.h"
#include "omegaWTK/Core/Core.h"

#include "omega-common/net.h"

#include "../Core/AssetsPriv.h"

#include <sstream>



#include <iostream>
#include <fstream>
#include <memory>

#include "ImgCodecPriv.h"



namespace OmegaWTK::Media {




Core::UniquePtr<ImgCodec> getPngCodec(Core::IStream &in,BitmapImage *img);
Core::UniquePtr<ImgCodec> getJpegCodec(Core::IStream &in,BitmapImage *img);
Core::UniquePtr<ImgCodec> getTiffCodec(Core::IStream &in,BitmapImage *img);

Core::UniquePtr<ImgCodec> obtainCodecForImageFormat(BitmapImage::Format &format,Core::IStream &in,BitmapImage *img){
    switch (format) {
        case BitmapImage::PNG:
        {
            return getPngCodec(in,img);
            break;
        }
        case BitmapImage::TIFF:
        {
            
            return getTiffCodec(in,img);
            break;
        }
        case BitmapImage::JPEG:
        {
            return getJpegCodec(in,img);
            break;
        }
        default:
            return nullptr;
            break;
    }
    
   
};

    struct ImgBuffer : public std::streambuf {
        ImgBuffer(void *begin,void *end){
            this->setg((char *)begin,(char *)begin,(char *)end);
        };
    };

    StatusWithObj<BitmapImage> loadImageFromAssets(OmegaCommon::FS::Path path){
        BitmapImage img;
        auto ext = path.ext();
        BitmapImage::Format f;
        if(ext == "png"){
            f = BitmapImage::PNG;
        };
        auto & asset = AssetFileLoader::assets_res[path.str()];
        ImgBuffer buffer (asset.data,(char *)asset.data + asset.filesize);
        std::istream in(&buffer);
        Core::UniquePtr<ImgCodec> codec = obtainCodecForImageFormat(f,in,&img);
        codec->readToStorage();
        return std::move(img);
    };
    
    StatusWithObj<BitmapImage> loadImageFromFile(OmegaCommon::FS::Path path) {
        BitmapImage img;
        auto ext = path.ext();
        BitmapImage::Format f;
        auto os_corrected_path = path.absPath();
        if(ext == "png"){
            f = BitmapImage::PNG;
        };
        std::ifstream in(os_corrected_path,std::ios::binary);
        if(in.is_open()){
            Core::UniquePtr<ImgCodec> codec = obtainCodecForImageFormat(f,in,&img);
            codec->readToStorage();
            if(img.data == nullptr)
                return {"Failed to Load Image from File"};
            return std::move(img);
        }
        else
            return {"Failed to Load Image from File"};
    };

    std::shared_ptr<OmegaCommon::HttpClientContext> http_client;

    StatusWithObj<BitmapImage> loadImageFromURL(OmegaCommon::StrRef url,BitmapImage::Format format){
        if(!http_client){
            http_client = OmegaCommon::HttpClientContext::Create();
        }
        auto resp = http_client->makeRequest({url}).get();
        return loadImageFromBuffer((ImgByte *)resp.data,resp.size,format);
    }

    StatusWithObj<BitmapImage> loadImageFromBuffer(ImgByte *bufferData,size_t bufferSize,BitmapImage::Format f){
        BitmapImage img;
        ImgBuffer imgBuffer {bufferData,bufferData + bufferSize};
        std::istream in(&imgBuffer);
        Core::UniquePtr<ImgCodec> codec = obtainCodecForImageFormat(f,in,&img);
        codec->readToStorage();
        if(!img.data)
            return {"Failed to Load Image from File"};
        return std::move(img);
    };
};

