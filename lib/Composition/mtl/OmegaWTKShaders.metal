
#include <metal_stdlib>
#include "MetalBridgeHeader.h"

using namespace metal;

/**
 Solid Color Shaders.
 */

struct SolidColorRasterData {
    simd_float4 pos [[position]];
    simd_float4 color;
};

vertex SolidColorRasterData solidColorVertex(unsigned v_id[[vertex_id]],constant MTLBDSolidColorVertex *vertices[[buffer(OMEGAWTK_METAL_VERICES_BUFFER)]]){
    MTLBDSolidColorVertex _v = vertices[v_id];
    SolidColorRasterData out;
    out.pos = simd_float4(_v.position,0,1.0);
    out.color = _v.color;
    return out;
};

fragment simd_float4 solidColorFragment(SolidColorRasterData interpolated [[stage_in]]){
    return interpolated.color;
};

/**
 Texture 2D Shaders.
 */

struct Texture2DRasterData {
    simd_float4 pos [[position]];
//    simd_float4 color;
    simd_float2 texture2dCoordinate;
};

vertex Texture2DRasterData texture2DVertex(unsigned v_id [[vertex_id]],constant MTLBDTexture2DVertex *vertices [[buffer(OMEGAWTK_METAL_VERICES_BUFFER)]]){
    MTLBDTexture2DVertex _v = vertices[v_id];
    Texture2DRasterData out;
    out.pos = simd_float4(_v.position,0,1.0);
    out.texture2dCoordinate = _v.textureCoord;
    return out;
};


fragment simd_float4 texture2DFragment(Texture2DRasterData interpolated [[stage_in]],texture2d<half> texture [[texture(OMEGAWTK_METAL_TEXTURE_BUFFER)]]){
    constexpr sampler textureSampler (filter::linear);
    const simd_half4 textureColor = texture.sample(textureSampler,interpolated.texture2dCoordinate);
    return simd_float4(textureColor);
}


fragment simd_float4 texture2DFragmentWithBkgrd(Texture2DRasterData interpolated [[stage_in]],texture2d<half> texture [[texture(OMEGAWTK_METAL_TEXTURE_BUFFER)]],constant simd_float4 * backgroundColor [[buffer(OMEGAWTK_METAL_BACKGROUND_COLOR_BUFFER)]]){
    constexpr sampler textureSampler (filter::linear);
    const simd_half4 textureColor = texture.sample(textureSampler,interpolated.texture2dCoordinate);
    const simd_float4 color = simd_float4(textureColor);
    if(color.a < 1.0f){
        float intmd_alpha = 1.0f - color.a;
        simd_float3 intmd_a = simd_float3(color.r * color.a,color.g * color.a,color.b * color.a);
        simd_float3 intmd_b = simd_float3(backgroundColor->r * intmd_alpha,backgroundColor->g * intmd_alpha,backgroundColor->b * intmd_alpha);
        
        return simd_float4(intmd_a.r + intmd_b.r,intmd_a.g + intmd_b.g,intmd_a.b + intmd_b.b,1.0f);
    }
    else {
        return color;
    }
};
