add_omega_wtk_lib(
    STATIC
    NAME 
    "Core"
    SOURCES 
    "Core.cpp"
    "ImgCodec.cpp"
    HEADERS
    "Core.h"
    "Math.h"
    "ImgCodec.h"
)

target_include_directories("omegaWTKCore" PRIVATE "${PNG_INCLUDE_DIRS}")
target_link_libraries("omegaWTKCore" PRIVATE ${PNG_LIBRARIES})
target_compile_definitions("omegaWTKCore" PRIVATE "${PNG_DEFINTIONS}")