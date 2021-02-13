add_omega_wtk_lib(
    STATIC
    NAME 
    "Composition"
    SOURCES 
    "Backend.cpp"
    "Compositor.cpp"
    "Color.cpp"
    "Layer.cpp"
	"Text.cpp"
    "Visual.cpp"
    HEADERS 
    "Backend.h"
    "Compositor.h"
    "Color.h"
    "Layer.h"
	"Text.h"
    "Visual.h"
    LINK_LIBS 
    "omegaWTKCore"
    "omegaWTKNative"
)
# Exposing private_include from omegaWTKNative lib

target_include_directories("omegaWTKComposition" PRIVATE "${CMAKE_SOURCE_DIR}/lib/Native/private_include")

if(TARGET_WIN32)
    set(COMPOSITION_WIN_SOURCES "DirectXBackend.h" "DirectXBackend.cpp" "DCVisualTree.h" "DCVisualTree.cpp")
    list(TRANSFORM COMPOSITION_WIN_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/win/")
    target_sources("omegaWTKComposition" PRIVATE ${COMPOSITION_WIN_SOURCES})
endif()

if(TARGET_MACOS)
    set(COMPOSITION_MAC_SOURCES "QuartzBackend.h" "QuartzBackend.mm")
    list(TRANSFORM COMPOSITION_MAC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/macos/")
    target_sources("omegaWTKComposition" PRIVATE ${COMPOSITION_MAC_SOURCES})
endif()
