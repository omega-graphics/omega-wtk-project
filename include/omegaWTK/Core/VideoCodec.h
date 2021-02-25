#include "Core.h"

#ifndef OMEGAWTK_CORE_VIDEOCODEC_H
#define OMEGAWTK_CORE_VIDEOCODEC_H
namespace OmegaWTK::Core {

struct Video {
    
};
SharedPtr<Video> loadVideoFromFile(const FSPath &path);
};

#endif
