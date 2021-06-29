#include "omegaWTK/Core/Core.h"

namespace OmegaWTK {
    class AssetFileLoader {
    public:
        
        struct AssetBuffer {
            size_t filesize;
            void *data;
        };
        static OmegaCommon::Map<OmegaCommon::String,AssetBuffer> assets_res;
        static void loadAssetFile(OmegaCommon::FS::Path & path);
    };
};