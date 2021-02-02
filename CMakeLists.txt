cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project("OmegaWTK" C CXX)
set(OMEGAWTK_VERSION "0.1")

include_directories("include")

set(OmegaWTKLibs "" CACHE INTERNAL "OmegaWTK Libs")

if(XCODE OR MSVC_IDE)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(OmegaWTKUtils)

function(add_omega_wtk_lib)
    cmake_parse_arguments("_ARG" "STATIC;SHARED" "NAME" "SOURCES;HEADERS;LINK_LIBS" ${ARGN})

    string(PREPEND _ARG_NAME "omegaWTK")

    if(_ARG_HEADERS)
        file(RELATIVE_PATH HEADER_DIR "${CMAKE_SOURCE_DIR}/lib" ${CMAKE_CURRENT_SOURCE_DIR})
        list(TRANSFORM _ARG_HEADERS PREPEND "${CMAKE_SOURCE_DIR}/include/omegaWTK/${HEADER_DIR}/")
    endif()

    if(_ARG_STATIC)
        add_library(${_ARG_NAME} STATIC ${_ARG_SOURCES} ${_ARG_HEADERS})
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
    target_link_libraries(${_ARG_NAME} PRIVATE ${_ARG_LINK_LIBS})

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
    find_macos_framework("AVFoundation")
    find_macos_framework("AVKit")
    find_macos_framework("QuartzCore")
    # if(NOT XCODE)
    # find_path(LIBCXX_PATH "XcodeDefault.xctoolchain" REQUIRED)
    # message("LIBCXX_PATH:${LIBCXX_PATH}")
        
    # endif()
endif()

add_subdirectory("lib")

install(TARGETS ${OmegaWTKLibs} EXPORT OmegaWTKTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

export(EXPORT OmegaWTKTargets 
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/OmegaWTKTargets.cmake"
    NAMESPACE OmegaWTK::
)

configure_file(cmake/OmegaWTKConfig.cmake.in "${CMAKE_BINARY_DIR}/cmake/OmegaWTKConfig.cmake" @ONLY)
configure_file(cmake/OmegaWTKUTils.cmake "${CMAKE_BINARY_DIR}/cmake/OmegaWTKUtils.cmake" COPYONLY)
