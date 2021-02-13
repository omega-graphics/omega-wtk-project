#include "Core.h"

#ifndef OMEGAWTK_CORE_IMGCODEC_H
#define OMEGAWTK_CORE_IMGCODEC_H

namespace OmegaWTK {
    namespace Core {
        struct BitmapImage {
            unsigned width;
            unsigned height;
            void *data;
            unsigned stride;
            unsigned bitDepth;
            unsigned channelCount;
        };
        BitmapImage loadImageFromFile(const Core::String & name);
    };
};

#endif
