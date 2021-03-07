add_omega_wtk_lib(
    STATIC
    NAME 
    "Composition"
    SOURCES 
    "BackendPriv.h"
    "BackendPriv.cpp"
    "Backend.cpp"
    "Compositor.cpp"
    "Color.cpp"
    "Layer.cpp"
	"Text.cpp"
    "Visual.cpp"
    "Brush.cpp"
    "GeometricGraphicsPath.cpp"
    # BD Prefix --> For Backend!
    "BDCompositionDevice.h"
    "BDCompositionDevice.cpp"
    "BDCompositionImage.h"
    "BDCompositionImage.cpp"
    "BDCompositionRenderTarget.h"
    "BDCompositionRenderTarget.cpp"
    "BDCompositionFontFactory.h"
    "BDCompositionFontFactory.cpp"
    "BDCompositionVisualTree.h"
    "BDCompositionVisualTree.cpp"
    HEADERS 
    "Backend.h"
    "Compositor.h"
    "Color.h"
    "Layer.h"
	"Text.h"
    "Visual.h"
    "Brush.h"
    "GeometricGraphicsPath.h"
    LINK_LIBS 
    "omegaWTKCore"
    "omegaWTKNative"
)
# Exposing private_include from omegaWTKNative lib

target_include_directories("omegaWTKComposition" PRIVATE "${CMAKE_SOURCE_DIR}/lib/Native/private_include")

if(TARGET_WIN32)
    message("Windows Ver:${CMAKE_HOST_SYSTEM_VERSION}")
    if(${CMAKE_HOST_SYSTEM_VERSION} GREATER_EQUAL 10)
        target_compile_definitions("omegaWTKComposition" PRIVATE DIRECT3D_12)
    # elseif(${CMAKE_HOST_SYSTEM_VERSION} LESS 10)

    endif()
    # set(COMPOSITION_WIN_SOURCES "DirectXBackend.h" "DirectXBackend.cpp" "DCVisualTree.h" "DCVisualTree.cpp")
    set(COMPOSITION_WIN_SOURCES 
    "DXBDCompositionDevice.h" 
    "DXBDCompositionDevice.cpp" 
    "DXBDCompositionRenderTarget.h" 
    "DXBDCompositionRenderTarget.cpp" 
    "DXBDCompositionImage.h" 
    "DXBDCompositionImage.cpp" 
    "DXBDCompositionFontFactory.h" 
    "DXBDCompositionFontFactory.cpp"
    "DCVisualTree.h"
    "DCVisualTree.cpp")
    list(TRANSFORM COMPOSITION_WIN_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/dx/")
    target_sources("omegaWTKComposition" PRIVATE ${COMPOSITION_WIN_SOURCES})
endif()

if(TARGET_MACOS)
    # set(COMPOSITION_MAC_SOURCES "QuartzBackend.h" "QuartzBackend.mm" "MetalBackend.h" "MetalBackend.mm" "MetalBridgeHeader.h" "OmegaWTKShaders.metal")
    set(COMPOSITION_MAC_SOURCES 
    "MTLBackend.h" 
    "MTLBDCompositionDevice.h" 
    "MTLBDCompositionDevice.mm" 
    "MTLBDCompositionRenderTarget.h"
    "MTLBDCompositionRenderTarget.mm"
    "MTLBDTriangulator.h"
    "MTLBDTriangulator.mm"
    "MTLBDCompositionImage.h"
    "MTLBDCompositionImage.mm"
    "MTLBDCompositionFontFactory.h"
    "MTLBDCompositionFontFactory.mm"
    "MTLBDCALayerTree.h"
    "MTLBDCALayerTree.mm"
    "MetalBridgeHeader.h" 
    "OmegaWTKShaders.metal")
    list(TRANSFORM COMPOSITION_MAC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/mtl/")
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/mtl/OmegaWTKShaders.metal" PROPERTIES LANGUAGE METAL)
    target_link_libraries("omegaWTKComposition" PRIVATE ${Metal_LIB} ${MetalPerformanceShaders_LIB})
    target_sources("omegaWTKComposition" PRIVATE ${COMPOSITION_MAC_SOURCES})
endif()
