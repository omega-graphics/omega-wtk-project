add_omega_wtk_lib(
    STATIC
    NAME 
    "Core"
    SOURCES 
    "Core.cpp"
    "ImgCodec.cpp"
    "VideoCodec.cpp"
    HEADERS
    "Core.h"
    "Math.h"
    "ImgCodec.h"
    "VideoCodec.h"
)

if(TARGET_MACOS)
    target_sources("omegaWTKCore" PRIVATE "FSCocoa.h" "FSCocoa.mm")
    target_link_libraries("omegaWTKCore" PRIVATE ${Cocoa_LIB})
endif()
message("libpng_include_dir:${PNG_INCLUDE_DIR}")
target_include_directories("omegaWTKCore" PRIVATE ${PNG_INCLUDE_DIR}${TIFF_INCLUDE_DIR}${TIFF_INCLUDE_DIRS}${JPEG_INCLUDE_DIR})
target_link_libraries("omegaWTKCore" PRIVATE "PNG::PNG;TIFF::TIFF;${JPEG_LIBRARIES}") #${FFMPEG_INCLUDE_DIRS} ${FFMPEG_LIBRARIES}
target_compile_definitions("omegaWTKCore" PRIVATE "${PNG_DEFINTIONS}")