#include "omegaWTK/Core/Core.h"

namespace OmegaWTK {
    class AssetFileLoader {
    public:
        
        struct AssetBuffer {
            size_t filesize;
            void *data;
        };
        static Core::Map<Core::String,AssetBuffer> assets_res;
        static void loadAssetFile(FS::Path & path);
    };
};