add_omega_wtk_lib(
    OBJECT
    NAME 
    "Composition"
    SOURCES 
    "Compositor.cpp"
    "Color.cpp"
    "Layer.cpp"
    "Canvas.cpp"
    "Brush.cpp"
    "GeometricGraphicsPath.cpp"
    "LayerAnimation.cpp"
    "CompositorClient.cpp"
    "Compositor.h"
    # BD Prefix --> For Backend!
    HEADERS 
    "Color.h"
    "Layer.h"
    "Canvas.h"
    "Brush.h"
    "GeometricGraphicsPath.h"
    "LayerAnimation.h"
    "CompositorClient.h"
    "FontEngine.h"
    LINK_LIBS 
    "omegaWTKCore"
    "omegaWTKNative"
    "omegaWTKMedia"
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
    "DCVisualTree.h"
    "DCVisualTree.cpp")
    list(TRANSFORM COMPOSITION_WIN_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/dx/")
    target_sources("omegaWTKComposition" PRIVATE ${COMPOSITION_WIN_SOURCES} "DWriteFontEngine.cpp")
endif()

if(TARGET_MACOS)
    # set(COMPOSITION_MAC_SOURCES "QuartzBackend.h" "QuartzBackend.mm" "MetalBackend.h" "MetalBackend.mm" "MetalBridgeHeader.h" "OmegaWTKShaders.metal")
    set(COMPOSITION_MAC_SOURCES 
    "MTLBDCALayerTree.h"
    "MTLBDCALayerTree.mm")
    list(TRANSFORM COMPOSITION_MAC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/mtl/")
    # set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/mtl/OmegaWTKShaders.metal" PROPERTIES LANGUAGE METAL)
    target_link_libraries("omegaWTKComposition" PRIVATE ${Metal_LIB} ${MetalPerformanceShaders_LIB})

    target_sources("omegaWTKComposition" PRIVATE ${COMPOSITION_MAC_SOURCES} "CTFontEngine.mm")
endif()
