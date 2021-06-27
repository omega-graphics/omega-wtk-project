cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

cmake_policy(SET CMP0077 NEW)

set(BUILD_TESTING OFF)

# set(BUILD_AS_CPP20MODULE)

if(BUILD_AS_CPP20MODULE AND NOT ${CMAKE_CXX_STANDARD} STREQUAL 20)
    message(FATAL_ERROR "CXX Standard must be c++20 if building the Module OmegaWTK")
endif()
if(BUILD_AS_CPP20MODULE)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fmodules")
    add_compile_definitions(OMEGAWTK_BUILD_MODULE)
endif()

# Include Dependencies

add_subdirectory("gte")

project("OmegaWTK" C CXX)
set(OMEGAWTK_VERSION "0.4")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-gdwarf")
endif()

include(GNUInstallDirs)

# 3rd Party Libraries

find_package(ZLIB)
if(ZLIB_FOUND)
    message("Found zlib:${ZLIB_VERSION_STRING}")
else()
    add_subdirectory("external-libs/zlib/code")
endif()

find_package(PNG)
if(PNG_FOUND)
    message("Found libpng:${PNG_VERSION_STRING}")
else()
    set(PNG_TESTS OFF)
    add_subdirectory("external-libs/libpng/code")
    # message(FATAL_ERROR "libpng not found! libpng is required to build OmegaWTK")
endif()

find_package(JPEG)
if(JPEG_FOUND)
    message("Found libjpeg-turbo:${JPEG_VERSION_STRING}")
else()
    add_subdirectory("external-libs/libjpeg-turbo/code")
endif()

find_package(TIFF)
if(TIFF_FOUND)
    message("Found libtiff")
else()
    add_subdirectory("external-libs/libtiff/code")
endif()

# find_package(OpenSSL REQUIRED)
# if(OpenSSL_FOUND)
#     message("Found OpenSSL")
# else()
#     add_subdirectory("external-libs/openssl/code")
# endif()
    
include_directories("include")
include(ExternalProject)



set(OmegaWTKLibs "" CACHE INTERNAL "OmegaWTK Libs")

if(XCODE OR MSVC_IDE)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(OmegaWTKUtils)
# include(FindFFMPEG)

# find_package(FFMPEG)
# if(FFMPEG_FOUND)
#     message("Found FFMPEG")
# endif()

set(OMEGAWTK_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(add_omega_wtk_lib)
    cmake_parse_arguments("_ARG" "STATIC;SHARED;OBJECT" "NAME" "SOURCES;HEADERS;LINK_LIBS" ${ARGN})

    string(PREPEND _ARG_NAME "omegaWTK")

    if(_ARG_HEADERS)
        file(RELATIVE_PATH HEADER_DIR "${OMEGAWTK_ROOT_DIR}/src" ${CMAKE_CURRENT_SOURCE_DIR})
        list(TRANSFORM _ARG_HEADERS PREPEND "${OMEGAWTK_ROOT_DIR}/include/omegaWTK/${HEADER_DIR}/")
    endif()

    if(_ARG_STATIC)
        add_library(${_ARG_NAME} STATIC ${_ARG_SOURCES} ${_ARG_HEADERS})
    elseif(_ARG_OBJECT)
        add_library(${_ARG_NAME} OBJECT ${_ARG_SOURCES} ${_ARG_HEADERS})
    elseif(_ARG_SHARED)
        add_library(${_ARG_NAME} SHARED ${_ARG_SOURCES} ${_ARG_HEADERS})
    endif()


    set(OmegaWTKLibs ${OmegaWTKLibs} ${_ARG_NAME} CACHE INTERNAL "OmegaWTK Libs")

    set_target_properties(${_ARG_NAME} PROPERTIES
     VERSION ${OMEGAWTK_VERSION}
     INTERFACE_${_ARG_NAME}_MAJOR_VERSION ${OMEGAWTK_VERSION}
     ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
     FOLDER "OmegaWTK Libraries")

    if(NOT TARGET ${_ARG_NAME})
        message(FATAL_ERROR "OmegaWTK Lib:${_ARG_NAME}, does not have a type!")
    endif()

    foreach(_LIB IN ITEMS ${_ARG_LINK_LIBS})
        add_dependencies(${_ARG_NAME} ${_LIB})
    endforeach()
    target_link_libraries(${_ARG_NAME} PUBLIC ${_ARG_LINK_LIBS})

endfunction()

if(WIN32)
    message("Targeting Win32")
endif()

if(CMAKE_HOST_APPLE)
    enable_language(OBJCXX Swift)
    message("Targeting macOS")

    find_macos_framework("Cocoa")
    find_macos_framework("Metal")
    find_macos_framework("MetalPerformanceShaders")
    find_macos_framework("AVFoundation")
    find_macos_framework("AVKit")
    find_macos_framework("QuartzCore")
    # if(NOT XCODE)
    # find_path(LIBCXX_PATH "XcodeDefault.xctoolchain" REQUIRED)
    # message("LIBCXX_PATH:${LIBCXX_PATH}")
        
    # endif()
endif()

if(TARGET_GTK)
    message("Targeting GTK+3/Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
    if(GTK_FOUND)
        
    endif()
    find_package(Vulkan REQUIRED)
    if(Vulkan_FOUND)

    endif()
endif()

add_subdirectory("src")

set(OMEGAWTK_TARGET_UTILS_DIR ${CMAKE_SOURCE_DIR}/target)

add_subdirectory("tests")

if(TARGET_WIN32 OR TARGET_GTK)
    install(TARGETS ${OmegaWTKLibs} EXPORT OmegaWTKTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

if(TARGET_MACOS)
    install(TARGETS ${OmegaWTKLibs} EXPORT OmegaWTKTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        # FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

export(TARGETS ${OmegaWTKLibs} 
    FILE "${CMAKE_BINARY_DIR}/lib/cmake/OmegaWTKTargets.cmake"
    NAMESPACE OmegaWTK::
)

install(EXPORT OmegaWTKTargets
    NAMESPACE OmegaWTK:: 
    DESTINATION "${CMAKE_BINARY_DIR}/lib/cmake")

configure_file(cmake/OmegaWTKConfig.cmake.in "${CMAKE_BINARY_DIR}/lib/cmake/OmegaWTKConfig.cmake" @ONLY)
configure_file(cmake/OmegaWTKUtils.cmake "${CMAKE_BINARY_DIR}/lib/cmake/OmegaWTKUtils.cmake" COPYONLY)

