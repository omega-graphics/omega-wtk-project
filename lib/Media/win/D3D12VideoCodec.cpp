#include "omegaWTK/Media/VideoCodec.h"

#include <d3d12video.h>
#include <mfmediaengine.h>

#include "../VideoCodec.cpp"

namespace OmegaWTK::Media {

    class D3D12VideoCodec : public VideoCodec {
        ID3D12VideoDevice2 *device;
    public:
    };

};