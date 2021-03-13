
#include <metal_stdlib>
#include "MetalBridgeHeader.h"

using namespace metal;

//kernel void locateThreadInThread

/**
 Gradient Texture Generators
 */
kernel void linearGradientKernel(constant MTLBDGradientStop *gradientStops [[buffer(OMEGAWTK_METAL_GRADIENT_STOP_BUFFER)]],
                                 constant unsigned *stopsBufferLen [[buffer(OMEGAWTK_METAL_GRADIENT_STOP_BUFFER_LEN)]],
                                 texture2d<half,access::write> out [[texture(OMEGAWTK_METAL_GRADIENT_TEXTURE_OUT)]],
                                 uint2 gid [[thread_position_in_grid]]){
    auto stops = make_array_ref(gradientStops,*stopsBufferLen);
    if((gid.x >= out.get_width()) || (gid.y >= out.get_height())){
        return;
    };
    
    auto relativeXPos = float(gid.x)/float(out.get_width());
    auto relativeYPos = float(gid.y)/float(out.get_height());
    
    
    simd_half4 outColor;
    out.write(outColor,gid);
    
};

kernel void radialGradientKernel(constant MTLBDGradientStop *gradientStops [[buffer(OMEGAWTK_METAL_GRADIENT_STOP_BUFFER)]],
                                 constant unsigned *stopsBufferLen [[buffer(OMEGAWTK_METAL_GRADIENT_STOP_BUFFER_LEN)]],
                                 texture2d<half,access::write> out [[texture(OMEGAWTK_METAL_GRADIENT_TEXTURE_OUT)]],
                                 uint2 gid [[thread_position_in_grid]]){
    auto stops = make_array_ref(gradientStops,*stopsBufferLen);
    if((gid.x >= out.get_width()) || (gid.y >= out.get_height())){
        return;
    };
    
};

/**
 Texture Masking Function
 */

kernel void alphaMaskKernel(texture2d<half,access::read> main_in [[texture(OMEGAWTK_METAL_MASK_TEXTURE_MAIN)]],
                        texture2d<half,access::read> mask [[texture(OMEGAWTK_METAL_MASK_TEXTURE_MASK)]],
                        texture2d<half,access::write> out [[texture(OMEGAWTK_METAL_MASK_TEXTURE_OUT)]],
                        uint2 gid [[thread_position_in_grid]]){
    if(gid.x >= main_in.get_width() || gid.y >= main_in.get_width()){
        return;
    }
    else if(gid.x >= mask.get_width() || gid.y >= main_in.get_height()){
        simd_half4 main_pt = main_in.read(gid);
        main_pt.a *= 0.f;
        out.write(main_pt,gid);
        return;
    };
    simd_half4 main_pt = main_in.read(gid);
    simd_half4 mask_pt = mask.read(gid);
    main_pt.a *= mask_pt.a;
    out.write(main_pt,gid);
    
};

/**
 Solid Color Shaders.
 */

struct SolidColorRasterData {
    simd_float4 pos [[position]];
    simd_float4 color;
};

vertex SolidColorRasterData solidColorVertex(unsigned v_id[[vertex_id]],
                                             constant MTLBDSolidColorVertex *vertices[[buffer(OMEGAWTK_METAL_VERICES_BUFFER)]]){
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

vertex Texture2DRasterData texture2DVertex(unsigned v_id [[vertex_id]],
                                           constant MTLBDTexture2DVertex *vertices [[buffer(OMEGAWTK_METAL_VERICES_BUFFER)]]){
    MTLBDTexture2DVertex _v = vertices[v_id];
    Texture2DRasterData out;
    out.pos = simd_float4(_v.position,0,1.0);
    out.texture2dCoordinate = _v.textureCoord;
    return out;
};


fragment simd_float4 texture2DFragment(Texture2DRasterData interpolated [[stage_in]],
                                       texture2d<half> texture [[texture(OMEGAWTK_METAL_TEXTURE_BUFFER)]]){
    constexpr sampler textureSampler (filter::linear);
    const simd_half4 textureColor = texture.sample(textureSampler,interpolated.texture2dCoordinate);
    return simd_float4(textureColor);
}


fragment simd_float4 texture2DFragmentWithBkgrd(Texture2DRasterData interpolated [[stage_in]],
                                                texture2d<half> texture [[texture(OMEGAWTK_METAL_TEXTURE_BUFFER)]],
                                                constant simd_float4 * backgroundColor [[buffer(OMEGAWTK_METAL_BACKGROUND_COLOR_BUFFER)]]){
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
