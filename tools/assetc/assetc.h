#include <cstdint>

#ifndef OMEGAWTK_ASSETC_ASSETC_H
#define OMEGAWTK_ASSETC_ASSETC_H

namespace OmegaWTK::assetc {

struct AssetsFileHeader {
   unsigned asset_count;
};

struct AssetsFileEntry {
  uint64_t string_size;
  uint64_t file_size;
};

};

#endif