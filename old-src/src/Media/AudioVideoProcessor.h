#include "omegaWTK/Core/Core.h"

namespace OmegaWTK::Media {

    INTERFACE AudioVideoProcessor {
        INTERFACE_METHOD(void,beginEncode)
        INTERFACE_METHOD(void,endEncode)
        INTERFACE_METHOD(void,beginDecode)
        INTERFACE_METHOD(void,endDecode)
        INTERFACE_METHOD(void,encodeFrame)
        INTERFACE_METHOD(void,decodeFrame)
    };

    extern AudioVideoProcessor *avProcessor;

    
};