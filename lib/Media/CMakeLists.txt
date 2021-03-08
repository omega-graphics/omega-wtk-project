add_omega_wtk_lib(
    STATIC 
    NAME 
    "Media"
    SOURCES 
    "AudioCaptureSession.cpp"
    "VideoCaptureSession.cpp"
    "ImgCodec.cpp"
    "VideoCodec.cpp"
    HEADERS 
    "AudioCaptureSession.h"
    "VideoCaptureSession.h"
    "ImgCodec.h"
    "VideoCodec.h"
)

message("libpng_include_dir:${PNG_INCLUDE_DIR}")
target_include_directories("omegaWTKMedia" PRIVATE ${PNG_INCLUDE_DIR}${TIFF_INCLUDE_DIR}${TIFF_INCLUDE_DIRS}${FFMPEG_INCLUDE_DIRS})
target_link_libraries("omegaWTKMedia" PRIVATE "PNG::PNG;TIFF::TIFF;${FFMPEG_LIBRARIES}") #${JPEG_INCLUDE_DIR} ${JPEG_LIBRARIES}
target_compile_definitions("omegaWTKMedia" PRIVATE "${PNG_DEFINTIONS}")