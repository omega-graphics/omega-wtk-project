#include "Core.h"

#ifndef OMEGAWTK_CORE_IMGCODEC_H
#define OMEGAWTK_CORE_IMGCODEC_H

namespace OmegaWTK {
    namespace Core {
        struct ImgProfile {
            Core::String name;
            int compression_type;
        };
        struct ImgHeader;
    
        struct BitmapImage {
            UniquePtr<ImgProfile> profile;
            UniquePtr<ImgHeader> header;
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
            float gamma;
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
        SharedPtr<BitmapImage> loadImageFromFile(FSPath path);
    };
};

#endif
