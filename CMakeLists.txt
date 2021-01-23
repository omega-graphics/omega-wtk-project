cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

project("OmegaWTK" C CXX)

include_directories("include")

include(CMakeParseArguments)

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

    if(NOT TARGET ${_ARG_NAME})
        message(FATAL_ERROR "OmegaWTK Lib:${_ARG_NAME}, does not have a type!")
    endif()

    foreach(_LIB IN ITEMS ${_ARG_LINK_LIBS})
        add_dependencies(${_ARG_NAME} ${_LIB})
    endforeach()
    target_link_libraries(${_ARG_NAME} PRIVATE ${_ARG_LINK_LIBS})

endfunction()

# function(add_omega_wtk_app)
#     cmake_parse_arguments("_ARG" "" "NAME;SOURCES")

# endfunction()

if(WIN32)
    add_compile_definitions("TARGET_WIN32")
    set(TARGET_WIN32 TRUE)
endif()

if(CMAKE_HOST_APPLE)
    add_compile_definitions("TARGET_MACOS")
    set(TARGET_MACOS TRUE)
    enable_language(OBJCXX)
    # Locates macOS Framework. Outputs to ${F_NAME}_LIB
    function(find_macos_framework F_NAME)
        message(STATUS "Locating ${F_NAME} Framework")
        find_library("${F_NAME}_LIB" "${F_NAME}" REQUIRED)
        if(NOT ${F_NAME}_LIB)
            message(STATUS "Locating ${F_NAME} Framework - not found")
            message(FATAL_ERROR "${F_NAME}.framework is required to build OmegaWTK")
        else()
            message(STATUS "Locating ${F_NAME} Framework - found (Path:${${F_NAME}_LIB})")
        endif()
    endfunction()

    find_macos_framework("Cocoa")
    find_macos_framework("Metal")
    find_macos_framework("AVFoundation")
    find_macos_framework("AVKit")
    # if(NOT XCODE)
    # find_path(LIBCXX_PATH "XcodeDefault.xctoolchain" REQUIRED)
    # message("LIBCXX_PATH:${LIBCXX_PATH}")
        
    # endif()
endif()

add_subdirectory("lib")
