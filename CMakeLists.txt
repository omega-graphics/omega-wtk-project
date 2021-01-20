cmake_minimum_required(VERSION 3.13)

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

if(WIN32)
    add_compile_definitions("TARGET_WIN32")
endif()

if(CMAKE_HOST_APPLE)
    add_compile_definitions("TARGET_MACOS")
endif()

add_subdirectory("lib")
add_subdirectory("test")
