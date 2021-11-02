#include "omegaWTK/Core/Core.h"

#ifndef OMEGAWTK_MEDIA_MEDIAIO_H
#define OMEGAWTK_MEDIA_MEDIAIO_H

namespace OmegaWTK::Media {

    struct MediaBuffer {
        void *data;
        size_t length;
    };

    struct MediaInputStream {
        bool bufferOrFile;
        OmegaCommon::String file;
        MediaBuffer buffer;
        static MediaInputStream fromFile(const OmegaCommon::FS::Path & path);
        static MediaInputStream fromBuffer(void *data,size_t length);
    };

    struct MediaOutputStream {
        bool bufferOrFile;
        OmegaCommon::String file;
        MediaBuffer buffer;
        static MediaOutputStream toFile(const OmegaCommon::FS::Path & path);
        static MediaOutputStream toBuffer(void *data,size_t length);
    };
}
#endif