add_omega_wtk_lib(
    STATIC 
    NAME 
    "Composition"
    SOURCES 
    "Compositor.cpp"
    HEADERS 
    "Compositor.h"
    LINK_LIBS 
    "omegaWTKCore"
    "omegaWTKNative"
)
# Exposing private_include from omegaWTKNative lib!
target_include_directories("omegaWTKComposition" PRIVATE "${CMAKE_SOURCE_DIR}/lib/Native/private_include")

# if(TARGET_WIN32)
#     add_subdirectory("win")
# endif()

# if(TARGET_MACOS)
#     add_subdirectory("macos")
# endif()
