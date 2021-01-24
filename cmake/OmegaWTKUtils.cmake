
include(CMakeParseArguments)

if(WIN32)
    add_compile_definitions("TARGET_WIN32")
    set(TARGET_WIN32 TRUE)
endif()

if(CMAKE_HOST_APPLE)
    if(NOT XCODE)
        set(FATAL_ERROR "Xcode Generator is Needed to Build an OmegaWTK App For Mac!")
    endif()
    add_compile_definitions("TARGET_MACOS")
    set(TARGET_MACOS TRUE)
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

endif()

function(add_omega_wtk_app)
    cmake_parse_arguments("_ARG" "" "NAME;" "SOURCES;LINK_LIBS;INCLUDE_DIRS")

    if(TARGET_WIN32)
        add_executable(${_ARG_NAME} WIN32 ${_ARG_SOURCES})
        set(OMEGAWTK_WINDOWS_UTILS_DIR  ${OMEGAWTK_TARGET_UTILS_DIR}/windows)
        set(WINDOWS_UTIL_FILES "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.rc" "${CMAKE_CURRENT_BINARY_DIR}/targetver.h" "${CMAKE_CURRENT_BINARY_DIR}/resource.h")
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/resource_scripts.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.rc" COPYONLY)
        file(COPY "${OMEGAWTK_WINDOWS_UTILS_DIR}/resource.h;${OMEGAWTK_WINDOWS_UTILS_DIR}/targetver.h" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        target_sources(${_ARG_NAME} PRIVATE ${WINDOWS_UTIL_FILES})
    endif()

    if(TARGET_MACOS)
        if(NOT OBJCXX IN_LIST ENABLED_LANGUAGES)
            message(FATAL_ERROR "Objectve C++ Must be enabled!")
        endif()

        add_executable(${_ARG_NAME} MACOSX_BUNDLE ${_ARG_SOURCES})
        set(OMEGAWTK_MACOS_UTILS_DIR ${OMEGAWTK_TARGET_UTILS_DIR}/macos)
        set(MACOS_UTIL_FILES "AppDelegate.mm" "AppDelegate.h")
        list(TRANSFORM MACOS_UTIL_FILES PREPEND "${OMEGAWTK_TARGET_UTILS_DIR}/")
        file(COPY "${MACOS_UTIL_FILES}" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        set(MACOS_UTILS "AppDelegate.mm" "AppDelegate.h" "Info.plist")
        list(TRANSFORM MACOS_UTILS PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
        target_sources(${_ARG_NAME} PRIVATE ${MACOS_UTILS})
        # set_source_files_properties("Assets.xcassets" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        set_target_properties(${_ARG_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
        )
    endif()

    target_include_directories(${_ARG_NAME} PUBLIC ${_ARG_INCLUDE_DIRS})
    target_link_libraries(${_ARG_NAME} PRIVATE ${_ARG_LINK_LIBS})

endfunction()

function(omega_wtk_link_to_libs LIBS_TO_LINKS)

    foreach(lib ${LIBS_TO_LINKS})
        if(TARGET ${lib})
            target_link_libraries(${lib} PRIVATE ${OMEGAWTK_LIBS})
        endif()    
    endforeach()
    
endfunction()

