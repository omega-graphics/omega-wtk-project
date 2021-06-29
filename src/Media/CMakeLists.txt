add_omega_wtk_lib(
    OBJECT 
    NAME 
    "Media"
    SOURCES 
    "ImgCodec.cpp"
    "VideoCodec.cpp"
    HEADERS 
    "AudioCapture.h"
    "VideoCapture.h"
    "ImgCodec.h"
    "VideoCodec.h"
)

message("libpng_include_dir:${PNG_INCLUDE_DIR}")
add_dependencies("omegaWTKMedia" ZLIB PNG JPEG TIFF)
target_include_directories("omegaWTKMedia" PRIVATE "${PNG_INCLUDE_DIR};${TIFF_INCLUDE_DIR};${TIFF_INCLUDE_DIRS};${FFMPEG_INCLUDE_DIRS}")
target_link_libraries("omegaWTKMedia" PRIVATE PNG_LIB TIFFXX_LIB) #${JPEG_INCLUDE_DIR} ${JPEG_LIBRARIES}