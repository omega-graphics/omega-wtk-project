#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_IMGCODEC_H
#define OMEGAWTK_MEDIA_IMGCODEC_H

namespace OmegaWTK {
    namespace Media {
        struct ImgProfile {
            Core::String name;
            int compression_type;
        };
        struct ImgHeader;
    
        struct BitmapImage {
            Core::UniquePtr<ImgProfile> profile;
            Core::UniquePtr<ImgHeader> header;
            void *data;
            
            enum class ColorFormat : OPT_PARAM {
                RGB,
                RGBA,
                Pallete
            };
            enum class AlphaFormat : OPT_PARAM {
                Straight,
                Premultipled,
                Ingore
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
        Core::SharedPtr<BitmapImage> loadImageFromFile(FSPath path);
    #define IMPORT_IMG(img) OmegaWTK::Media::loadImageFromFile(OmegaWTK::Core::String("./assets/") + img)
    };
typedef Core::SharedPtr<Media::BitmapImage> SharedImageHandle;
};

#endif