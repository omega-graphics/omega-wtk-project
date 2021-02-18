cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# if(XCODE)
#     if(${XCODE_VERSION} STREQUAL 12.4)
#         set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX11.1.sdk" CACHE INTERNAL "CMAKE_OSX Root")
#     endif()
# endif()

project("OmegaWTK" C CXX)
set(OMEGAWTK_VERSION "0.1")

set(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "10.15")


# set(CMAKE_CXX_FLAGS "-gdwarf")

macro(find_vcpkg USE_TOOLCHAIN)
	message(STATUS "Locating vcpkg executable")
	find_program(VCPKG_ROOT "vcpkg" ONLY_CMAKE_FIND_ROOT_PATH)
	if(NOT EXISTS ${VCPKG_ROOT})
		message(STATUS "Locating vcpkg executable - not found")
	else()
		message(STATUS "Locating vcpkg executable - found")
		get_filename_component(VCPKG_DIR ${VCPKG_ROOT} DIRECTORY)
		message(STATUS "Locating vcpkg ports")
		if(IS_DIRECTORY ${VCPKG_DIR}/ports)
			message(STATUS "Locating vcpkg ports - found")
			if(${USE_TOOLCHAIN})
				message(STATUS "Locating vcpkg toolchain file")
				set(VCPKG_TOOLCHAIN_FILE_DIR ${VCPKG_DIR}/scripts/buildsystems)
				if(EXISTS ${VCPKG_TOOLCHAIN_FILE_DIR}/vcpkg.cmake)
					message(STATUS "Locating vcpkg toolchain file - found")
					message(STATUS "Loading toolchain")
					include(${VCPKG_TOOLCHAIN_FILE_DIR}/vcpkg.cmake)
				else()
					message(STATUS "Locating vcpkg toolchain file - not found")
				endif()
			endif()
		else()
			message(STATUS "Locating vcpkg ports - not found")
			message(FATAL_ERROR "vcpkg executable is provided but there are NO ports found in its directory!")
		endif()
	endif()
endmacro()

if(VCPKG_TOOLCHAIN_FILE_DIR)
    message(STATUS "`vcpkg` toolchain directory - provided")
    message(STATUS "Loading toolchain")
    include(${VCPKG_TOOLCHAIN_FILE_DIR})
else()
    find_vcpkg(TRUE)
endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    message("Found zlib - Version:${ZLIB_VERSION_STRING}")
endif()

find_package(PNG REQUIRED)
if(PNG_FOUND)
    message("Found libpng - Version:${PNG_VERSION_STRING}")
else()
    message(FATAL_ERROR "libpng not found! libpng is required to build OmegaWTK")
endif()
# find_package(JPEG REQUIRED)
# endif()
find_package(TIFF REQUIRED)
if(TIFF_FOUND)
    message("Found libtiff")
endif()

include_directories("include")
include(ExternalProject)



set(OmegaWTKLibs "" CACHE INTERNAL "OmegaWTK Libs")

if(XCODE OR MSVC_IDE)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(OmegaWTKUtils)

function(add_omega_wtk_lib)
    cmake_parse_arguments("_ARG" "STATIC;SHARED;OBJECT" "NAME" "SOURCES;HEADERS;LINK_LIBS" ${ARGN})

    string(PREPEND _ARG_NAME "omegaWTK")

    if(_ARG_HEADERS)
        file(RELATIVE_PATH HEADER_DIR "${CMAKE_SOURCE_DIR}/lib" ${CMAKE_CURRENT_SOURCE_DIR})
        list(TRANSFORM _ARG_HEADERS PREPEND "${CMAKE_SOURCE_DIR}/include/omegaWTK/${HEADER_DIR}/")
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

macro(find_vcpkg USE_TOOLCHAIN)
	message(STATUS "Locating vcpkg executable")
	find_program(VCPKG_ROOT "vcpkg" ONLY_CMAKE_FIND_ROOT_PATH)
	if(NOT EXISTS ${VCPKG_ROOT})
		message(STATUS "Locating vcpkg executable - not found")
	else()
		message(STATUS "Locating vcpkg executable - found")
		get_filename_component(VCPKG_DIR ${VCPKG_ROOT} DIRECTORY)
		message(STATUS "Locating vcpkg ports")
		if(IS_DIRECTORY ${VCPKG_DIR}/ports)
			message(STATUS "Locating vcpkg ports - found")
			if(${USE_TOOLCHAIN})
				message(STATUS "Locating vcpkg toolchain file")
				set(VCPKG_TOOLCHAIN_FILE_DIR ${VCPKG_DIR}/scripts/buildsystems)
				if(EXISTS ${VCPKG_TOOLCHAIN_FILE_DIR}/vcpkg.cmake)
					message(STATUS "Locating vcpkg toolchain file - found")
					message(STATUS "Loading toolchain")
					include(${VCPKG_TOOLCHAIN_FILE_DIR}/vcpkg.cmake)
				else()
					message(STATUS "Locating vcpkg toolchain file - not found")
				endif()
			endif()
		else()
			message(STATUS "Locating vcpkg ports - not found")
			message(FATAL_ERROR "vcpkg executable is provided but there are NO ports found in its directory!")
		endif()
	endif()
endmacro()

# find_vcpkg(TRUE)

# find_package(libpng CONFIG REQUIRED)
# find_package(freetype CONFIG REQUIRED)

if(WIN32)
    message("Targeting Win32")
endif()

if(CMAKE_HOST_APPLE)
    enable_language(OBJCXX)
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

add_subdirectory("lib")

set(OMEGAWTK_TARGET_UTILS_DIR ${CMAKE_SOURCE_DIR}/target)

add_subdirectory("tests")

if(TARGET_WIN32)
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
    FILE "${CMAKE_BINARY_DIR}/cmake/OmegaWTKTargets.cmake"
    NAMESPACE OmegaWTK::
)

install(EXPORT OmegaWTKTargets
    NAMESPACE OmegaWTK:: 
    DESTINATION "${CMAKE_BINARY_DIR}/cmake")

configure_file(cmake/OmegaWTKConfig.cmake.in "${CMAKE_BINARY_DIR}/cmake/OmegaWTKConfig.cmake" @ONLY)
configure_file(cmake/OmegaWTKUTils.cmake "${CMAKE_BINARY_DIR}/cmake/OmegaWTKUtils.cmake" COPYONLY)
