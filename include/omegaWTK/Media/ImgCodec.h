#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_IMGCODEC_H
#define OMEGAWTK_MEDIA_IMGCODEC_H

namespace OmegaWTK {
    namespace Media {
        struct ImgProfile {
            OmegaCommon::String name;
            int compression_type;
        };

        typedef unsigned char ImgByte;
    
        struct OMEGAWTK_EXPORT BitmapImage {
            ImgProfile profile;
            
            ImgByte *data;
            
            enum class ColorFormat : OPT_PARAM {
                RGB,
                RGBA,
                Pallete,
                Unknown
            };
            enum class AlphaFormat : OPT_PARAM {
                Straight,
                Premultipled,
                Ingore,
                Unknown
            };

            struct ImgHeader {
                uint32_t width;
                uint32_t height;
                int channels;
                int bitDepth;
                int compression_method;
                int interlace_type;
                BitmapImage::ColorFormat color_format;
                BitmapImage::AlphaFormat alpha_format;
                size_t stride;
            } header;
           
            bool sRGB;
            bool hasGamma;
            double gamma;
            typedef enum : OPT_PARAM {
                PNG,
                TIFF,
                JPEG
            } Format;
        };

        typedef BitmapImage::ImgHeader ImgHeader;
    
    
        
        OMEGAWTK_EXPORT StatusWithObj<BitmapImage> loadImageFromFile(OmegaCommon::FS::Path path);
        OMEGAWTK_EXPORT StatusWithObj<BitmapImage> loadImageFromAssets(OmegaCommon::FS::Path path);
        OMEGAWTK_EXPORT StatusWithObj<BitmapImage> loadImageFromBuffer(ImgByte *bufferData,size_t bufferSize,BitmapImage::Format f);
        OMEGAWTK_EXPORT StatusWithObj<BitmapImage> loadImageFromURL(OmegaCommon::StrRef url,BitmapImage::Format format);

    #define IMPORT_IMG(img) ::OmegaWTK::Media::loadImageFromAssets(img)
    };
};

#endif
