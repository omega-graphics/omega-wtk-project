#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_IMGCODEC_H
#define OMEGAWTK_MEDIA_IMGCODEC_H

namespace OmegaWTK {
    namespace Media {
        struct ImgProfile {
            OmegaCommon::String name;
            int compression_type;
        };
        struct ImgHeader;
    
        struct OMEGAWTK_EXPORT BitmapImage {
            Core::UniquePtr<ImgProfile> profile;
            Core::UniquePtr<ImgHeader> header;
            void *data;
            
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
            bool sRGB;
            bool hasGamma;
            double gamma;
            typedef enum : OPT_PARAM {
                PNG,
                TIFF,
                JPEG
            } Format;
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
        };
        OMEGAWTK_EXPORT StatusWithObj<BitmapImage> loadImageFromFile(OmegaCommon::FS::Path path);
        OMEGAWTK_EXPORT StatusWithObj<BitmapImage> loadImageFromAssets(OmegaCommon::FS::Path path);
    #define IMPORT_IMG(img) ::OmegaWTK::Media::loadImageFromAssets(img)
    };
};

#endif
