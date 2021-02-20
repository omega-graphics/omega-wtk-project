#include <simd/simd.h>

#ifndef OMEGAWTK_COMPOSITION_MACOS_METALBRIDGEHEADER_H
#define OMEGAWTK_COMPOSITION_MACOS_METALBRIDGEHEADER_H

#define OMEGAWTK_METAL_TEXTURE_BUFFER 0
#define OMEGAWTK_METAL_VERICES_BUFFER 1
#define OMEGAWTK_METAL_BACKGROUND_COLOR_BUFFER 2

typedef struct {
    simd_float2 position;
    simd_float2 textureCoord;
} OmegaWTKMetalVertex;

#endif
