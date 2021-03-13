#include <simd/simd.h>

#ifndef OMEGAWTK_COMPOSITION_MACOS_METALBRIDGEHEADER_H
#define OMEGAWTK_COMPOSITION_MACOS_METALBRIDGEHEADER_H

#define OMEGAWTK_METAL_TEXTURE_BUFFER 0x00
#define OMEGAWTK_METAL_VERICES_BUFFER 0x01
#define OMEGAWTK_METAL_BACKGROUND_COLOR_BUFFER 0x02
#define OMEGAWTK_METAL_GRADIENT_STOP_BUFFER 0x03
#define OMEGAWTK_METAL_GRADIENT_TEXTURE_OUT 0x04
#define OMEGAWTK_METAL_GRADIENT_STOP_BUFFER_LEN 0x05
#define OMEGAWTK_METAL_MASK_TEXTURE_MAIN 0x06
#define OMEGAWTK_METAL_MASK_TEXTURE_MASK 0x07
#define OMEGAWTK_METAL_MASK_TEXTURE_OUT 0x08

typedef struct {
    float linePos;
    simd_float4 color;
} MTLBDGradientStop;

typedef struct {
    simd_float2 position;
    simd_float4 color;
} MTLBDSolidColorVertex;

typedef struct {
    simd_float2 position;
    simd_float2 textureCoord;
} MTLBDTexture2DVertex;



#endif
