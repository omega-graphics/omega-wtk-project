# Distributed under the OSI-approved BSD 3-Clause License.
#
#.rst:
# FindFFMPEG
# --------
#
# Find the FFPMEG libraries
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# The following variables will be defined:
#
#  ``FFMPEG_FOUND``
#    True if FFMPEG found on the local system
#
#  ``FFMPEG_INCLUDE_DIRS``
#    Location of FFMPEG header files
#
#  ``FFMPEG_LIBRARY_DIRS``
#    Location of FFMPEG libraries
#
#  ``FFMPEG_LIBRARIES``
#    List of the FFMPEG libraries found
#
#

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)
include(CMakeFindDependencyMacro)

if(NOT FFMPEG_FOUND)

function(select_library_configurations_from_names)
  cmake_parse_arguments(_slc "" "BASENAME" "NAMES;NAMES_RELEASE;NAMES_DEBUG" ${ARGN})
  list(APPEND _slc_NAMES_RELEASE ${_slc_NAMES})
  list(APPEND _slc_NAMES_DEBUG ${_slc_NAMES})
  find_library(${_slc_BASENAME}_LIBRARY_RELEASE NAMES ${_slc_NAMES_RELEASE} PATHS ${_IMPORT_PREFIX}/lib/ NO_DEFAULT_PATH)
  find_library(${_slc_BASENAME}_LIBRARY_DEBUG NAMES ${_slc_NAMES_DEBUG} PATHS ${_IMPORT_PREFIX}/debug/lib/ NO_DEFAULT_PATH)
  select_library_configurations(${_slc_BASENAME})
  set(${_slc_BASENAME}_LIBRARIES ${${_slc_BASENAME}_LIBRARIES} PARENT_SCOPE)
endfunction()

function(select_library_configurations_from_targets)
  cmake_parse_arguments(_slc "" "BASENAME" "TARGETS" ${ARGN})
  foreach(_target ${_slc_TARGETS})
    get_target_property(_rel ${_target} IMPORTED_LOCATION_RELEASE)
    get_target_property(_dbg ${_target} IMPORTED_LOCATION_DEBUG)
    get_target_property(_deps ${_target} INTERFACE_LINK_LIBRARIES)
    list(APPEND ${_slc_BASENAME}_LIBRARY_RELEASE ${_rel})
    list(APPEND ${_slc_BASENAME}_LIBRARY_DEBUG ${_dbg})
    foreach(_dep ${_deps})
      if(TARGET ${_dep})
        get_target_property(_dep_rel ${_dep} IMPORTED_LOCATION_RELEASE)
        get_target_property(_dep_dbg ${_dep} IMPORTED_LOCATION_DEBUG)
        list(APPEND ${_slc_BASENAME}_LIBRARY_RELEASE ${_dep_rel})
        list(APPEND ${_slc_BASENAME}_LIBRARY_DEBUG ${_dep_dbg})
      endif()
    endforeach()
  endforeach()
  select_library_configurations(${_slc_BASENAME})
  set(${_slc_BASENAME}_LIBRARIES ${${_slc_BASENAME}_LIBRARIES} PARENT_SCOPE)
endfunction()

# for finding system libraries (e.g. Apple framework libraries)
function(find_platform_dependent_libraries)
  cmake_parse_arguments(_fpd "" "" "NAMES" ${ARGN})
  foreach(_name ${_fpd_NAMES})
    find_library(${_name}_LIBRARY ${_name} REQUIRED)
    list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${${_name}_LIBRARY})
  endforeach()
  set(FFMPEG_PLATFORM_DEPENDENT_LIBS ${FFMPEG_PLATFORM_DEPENDENT_LIBS} PARENT_SCOPE)
endfunction()

set(FFMPEG_VERSION "4.3.1")

find_dependency(Threads)
if(UNIX)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS -pthread)
endif()
if(UNIX AND NOT APPLE)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS -lX11)
endif()

if(OFF)
  find_dependency(BZip2)
  select_library_configurations_from_targets(BASENAME BZip2 TARGETS BZip2::BZip2)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${BZip2_LIBRARIES})
endif()

if(OFF)
  find_dependency(Iconv)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${Iconv_LIBRARIES})
endif()

if(OFF)
  select_library_configurations_from_names(BASENAME FDK NAMES fdk-aac libfdk-aac)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${FDK_LIBRARIES})
endif()

if(OFF)
  find_dependency(LibLZMA)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${LibLZMA_LIBRARIES})
endif()

if(OFF)
  find_dependency(mp3lame)
  select_library_configurations_from_targets(BASENAME mp3lame TARGETS mp3lame::mp3lame)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${mp3lame_LIBRARIES})
endif()

if(OFF)
  if(UNIX)
    list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS dl)
  endif()
endif()

if(OFF)
  find_dependency(OpenCL)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${OpenCL_LIBRARIES})
endif()

if(OFF)
  find_dependency(OpenSSL)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${OPENSSL_LIBRARIES})
endif()

if(OFF)
  find_dependency(Opus)
  select_library_configurations_from_targets(BASENAME Opus TARGETS Opus::opus)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${Opus_LIBRARIES})
endif()

if(OFF)
  find_dependency(SDL2)
  if(TARGET SDL2::SDL2-static)
    select_library_configurations_from_targets(BASENAME SDL2 TARGETS SDL2::SDL2-static)
  else()
    select_library_configurations_from_targets(BASENAME SDL2 TARGETS SDL2::SDL2)
  endif()
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${SDL2_LIBRARIES})
endif()

if(OFF)
  find_dependency(Snappy)
  select_library_configurations_from_targets(BASENAME Snappy TARGETS Snappy::snappy)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${Snappy_LIBRARIES})
endif()

if(OFF)
  select_library_configurations_from_names(BASENAME SOXR NAMES soxr)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${SOXR_LIBRARIES})
endif()

if(OFF)
  select_library_configurations_from_names(BASENAME SPEEX NAMES_RELEASE libspeex NAMES_DEBUG libspeexd)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${SPEEX_LIBRARIES})
endif()

if(OFF)
  find_dependency(Ogg)  # ensure Ogg::ogg is defined as a target
  find_dependency(unofficial-theora)
  select_library_configurations_from_targets(BASENAME THEORA TARGETS unofficial::theora::theoraenc)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${THEORA_LIBRARIES})
endif()

if(OFF)
  find_dependency(Vorbis)
  select_library_configurations_from_targets(BASENAME vorbis TARGETS Vorbis::vorbis Vorbis::vorbisenc)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${vorbis_LIBRARIES})
endif()

if(OFF)
  find_dependency(unofficial-libvpx)
  select_library_configurations_from_targets(BASENAME libvpx TARGETS unofficial::libvpx::libvpx)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${libvpx_LIBRARIES})
endif()

if(OFF)
  find_dependency(wavpack)
  select_library_configurations_from_targets(BASENAME wavpack TARGETS WavPack::wavpack)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${wavpack_LIBRARIES})
endif()

if(OFF)
  select_library_configurations_from_names(BASENAME X264 NAMES x264 libx264)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${X264_LIBRARIES})
  if(UNIX)
    list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS dl)
  endif()
endif()

if(OFF)
  select_library_configurations_from_names(BASENAME X265 NAMES x265 x265-static)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${X265_LIBRARIES})
endif()

if(OFF)
  find_dependency(ZLIB)
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS ${ZLIB_LIBRARIES})
endif()

#  Platform dependent libraries required by FFMPEG
if(WIN32)
  if(NOT CYGWIN)
    list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS wsock32 ws2_32 secur32 bcrypt strmiids Vfw32 Shlwapi mfplat mfuuid)
  endif()
else()
  list(APPEND FFMPEG_PLATFORM_DEPENDENT_LIBS m)
endif()

# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)
if(_IMPORT_PREFIX STREQUAL "/")
  set(_IMPORT_PREFIX "")
endif()

set(_IMPORT_PREFIX)

macro(FFMPEG_FIND varname shortname headername)
  if(NOT FFMPEG_${varname}_INCLUDE_DIRS)
    find_path(FFMPEG_${varname}_INCLUDE_DIRS NAMES lib${shortname}/${headername} ${headername} PATHS ${_IMPORT_PREFIX}/include NO_DEFAULT_PATH)
  endif()
  if(NOT FFMPEG_${varname}_LIBRARY)
    find_library(FFMPEG_${varname}_LIBRARY_RELEASE NAMES ${shortname} PATHS ${_IMPORT_PREFIX}/lib/ NO_DEFAULT_PATH)
    find_library(FFMPEG_${varname}_LIBRARY_DEBUG NAMES ${shortname}d ${shortname} PATHS ${_IMPORT_PREFIX}/debug/lib/ NO_DEFAULT_PATH)
    get_filename_component(FFMPEG_${varname}_LIBRARY_RELEASE_DIR ${FFMPEG_${varname}_LIBRARY_RELEASE} DIRECTORY)
    get_filename_component(FFMPEG_${varname}_LIBRARY_DEBUG_DIR ${FFMPEG_${varname}_LIBRARY_DEBUG} DIRECTORY)
    select_library_configurations(FFMPEG_${varname})
    set(FFMPEG_${varname}_LIBRARY ${FFMPEG_${varname}_LIBRARY} CACHE STRING "")
  endif()
  if (FFMPEG_${varname}_LIBRARY AND FFMPEG_${varname}_INCLUDE_DIRS)
    set(FFMPEG_${varname}_FOUND TRUE BOOL)
    list(APPEND FFMPEG_INCLUDE_DIRS ${FFMPEG_${varname}_INCLUDE_DIRS})
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_${varname}_LIBRARY})
    list(APPEND FFMPEG_LIBRARY_DIRS ${FFMPEG_${varname}_LIBRARY_RELEASE_DIR} ${FFMPEG_${varname}_LIBRARY_DEBUG_DIR})
  endif()
endmacro(FFMPEG_FIND)

if(APPLE)
  find_platform_dependent_libraries(NAMES VideoToolbox CoreServices CoreMedia CoreVideo)
  if(OFF)
    find_platform_dependent_libraries(NAMES OpenCL)
  endif()
  if(ON)
    find_platform_dependent_libraries(NAMES AVFoundation CoreFoundation CoreGraphics Foundation)
  endif()
  if(ON)
    find_platform_dependent_libraries(NAMES OpenGL AppKit CoreImage)
  endif()
  if(ON)
    find_platform_dependent_libraries(NAMES Security)
  endif()
  if(ON)
    find_platform_dependent_libraries(NAMES AudioToolbox)
  endif()
endif()

if(ON)
  FFMPEG_FIND(libavdevice   avdevice   avdevice.h)
endif()
if(ON)
  FFMPEG_FIND(libavfilter   avfilter   avfilter.h)
endif()
if(ON)
  FFMPEG_FIND(libavformat   avformat   avformat.h)
endif()
if(ON)
  FFMPEG_FIND(libavcodec    avcodec    avcodec.h)
endif()
if(ON)
  FFMPEG_FIND(libavresample avresample avresample.h)
endif()
if(ON)
  FFMPEG_FIND(libpostproc   postproc   postprocess.h)
endif()
if(ON)
  FFMPEG_FIND(libswresample swresample swresample.h)
endif()
if(ON)
  FFMPEG_FIND(libswscale    swscale    swscale.h)
endif()
FFMPEG_FIND(libavutil       avutil     avutil.h)

if (FFMPEG_libavutil_FOUND)
  list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIRS)
  list(REMOVE_DUPLICATES FFMPEG_LIBRARY_DIRS)
  set(FFMPEG_libavutil_VERSION "${FFMPEG_VERSION}" CACHE STRING "")

  if(FFMPEG_libavcodec_FOUND)
    set(FFMPEG_libavcodec_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()
  if(FFMPEG_libavdevice_FOUND)
    set(FFMPEG_libavdevice_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()
  if(FFMPEG_libavfilter_FOUND)
    set(FFMPEG_libavfilter_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()
  if(FFMPEG_libavformat_FOUND)
    set(FFMPEG_libavformat_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()
  if(FFMPEG_libavresample_FOUND)
    set(FFMPEG_libavresample_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()
  if(FFMPEG_libswresample_FOUND)
    set(FFMPEG_libswresample_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()
  if(FFMPEG_libswscale_FOUND)
    set(FFMPEG_libswscale_VERSION "${FFMPEG_VERSION}" CACHE STRING "")
  endif()

  list(APPEND FFMPEG_LIBRARIES
    ${FFMPEG_PLATFORM_DEPENDENT_LIBS}
  )

  set(FFMPEG_LIBRARY ${FFMPEG_LIBRARIES})

  set(FFMPEG_FOUND TRUE CACHE BOOL "")
  set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} CACHE STRING "")
  set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS} CACHE STRING "")
  set(FFMPEG_LIBRARY_DIRS ${FFMPEG_LIBRARY_DIRS} CACHE STRING "")
endif()

find_package_handle_standard_args(FFMPEG REQUIRED_VARS FFMPEG_LIBRARIES FFMPEG_LIBRARY_DIRS FFMPEG_INCLUDE_DIRS)

endif()
