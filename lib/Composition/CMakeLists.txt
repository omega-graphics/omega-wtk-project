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
)

# if(WIN32)
#     add_subdirectory("win")
# else()
#     add_subdirectory("macos")
# endif()
