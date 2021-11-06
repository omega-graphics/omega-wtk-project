
include(CMakeParseArguments)

if(WIN32)
    add_compile_definitions("TARGET_WIN32")
    set(TARGET_WIN32 TRUE)
endif()

if(CMAKE_HOST_APPLE)
    # if(NOT XCODE)
    #     set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_IDENTITY ${CODE_SIGNATURE})
    # endif()
    

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
elseif(CMAKE_HOST_UNIX)
    add_compile_definitions("TARGET_GTK")
    set(TARGET_GTK TRUE)
endif()

function(add_omega_wtk_app)
    cmake_parse_arguments("_ARG" "" 
    "NAME;
    MAC_BUNDLE_ID;
    WIN_ICO;"

    "SOURCES;
    LINK_LIBS;
    INCLUDE_DIRS" 
    ${ARGN})

    if(TARGET_WIN32)
        add_executable(${_ARG_NAME} WIN32 ${_ARG_SOURCES})
        add_custom_command(TARGET ${_ARG_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${OMEGAWTK_MAIN_LIB} ${CMAKE_CURRENT_BINARY_DIR}/OmegaWTK.dll)
        message("COMMAND:${CMAKE_COMMAND} -E copy ${OMEGAWTK_MAIN_LIB} ${CMAKE_CURRENT_BINARY_DIR}/OmegaWTK.dll")
        set(OMEGAWTK_WINDOWS_UTILS_DIR  ${OMEGAWTK_TARGET_UTILS_DIR}/windows)
        set(WINDOWS_UTIL_FILES "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.rc" "${CMAKE_CURRENT_BINARY_DIR}/targetver.h" "${CMAKE_CURRENT_BINARY_DIR}/resource.h" "${CMAKE_CURRENT_BINARY_DIR}/mmain.cpp" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.exe.manifest")
        set(APPNAME ${_ARG_NAME})
        configure_file(
            "${OMEGAWTK_WINDOWS_UTILS_DIR}/mmain.cpp.in" 
            "${CMAKE_CURRENT_BINARY_DIR}/mmain.cpp")
        configure_file(
            "${OMEGAWTK_WINDOWS_UTILS_DIR}/resource_script.rc.in" 
            "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.rc" 
            @ONLY)
        configure_file(
            "${OMEGAWTK_WINDOWS_UTILS_DIR}/resource.h.in" 
            "${CMAKE_CURRENT_BINARY_DIR}/resource.h" 
            @ONLY)
        configure_file(
            "${OMEGAWTK_WINDOWS_UTILS_DIR}/targetver.h" 
            "${CMAKE_CURRENT_BINARY_DIR}/targetver.h" 
            @ONLY)
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/app.ico" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.ico" COPYONLY)
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/app.exe.manifest" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.exe.manifest")
        file(COPY "${OMEGAWTK_WINDOWS_UTILS_DIR}/small.ico" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/mmain.cpp" PROPERTIES COMPILE_DEFINITIONS "WINDOWS_PRIVATE")
        target_sources(${_ARG_NAME} PRIVATE ${WINDOWS_UTIL_FILES})
    endif()

    if(TARGET_MACOS)
        
        set(OMEGAWTK_MACOS_UTILS_DIR ${OMEGAWTK_TARGET_UTILS_DIR}/macos)
        set(MACOS_UTIL_FILES "AppDelegate.h" "AppDelegate.mm" "English.lproj/MainMenu.xib" "main.mm" )
        list(TRANSFORM MACOS_UTIL_FILES PREPEND "${OMEGAWTK_MACOS_UTILS_DIR}/")
        foreach(_F IN ITEMS ${MACOS_UTIL_FILES})
            file(COPY ${_F} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        endforeach()
        set(APPNAME ${_ARG_NAME})
        set(BUNDLE_ID ${_ARG_MAC_BUNDLE_ID})
        configure_file( 
            "${OMEGAWTK_MACOS_UTILS_DIR}/AppDelegate.mm" 
            "${CMAKE_CURRENT_BINARY_DIR}/AppDelegate.mm" 
            @ONLY)
		
		configure_file(
            "${OMEGAWTK_MACOS_UTILS_DIR}/Info.plist.in" 
            "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" 
            @ONLY)
		
        set(MACOS_UTILS "AppDelegate.mm" "AppDelegate.h" "main.mm")
        list(TRANSFORM MACOS_UTILS PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
        add_app_bundle(NAME ${_ARG_NAME}  
            INFO_PLIST 
            ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
            RESOURCES 
            ${CMAKE_CURRENT_BINARY_DIR}/MainMenu.xib
            DEPS 
            ${_ARG_LINK_LIBS} 
            "OmegaWTK"
            LIBS 
            ${_ARG_LINK_LIBS} 
            ${Cocoa_LIB}
            EMBEDDED_FRAMEWORKS 
            "OmegaWTK"
            SOURCES 
            ${_ARG_SOURCES}
            ${MACOS_UTILS})
        # target_sources(${_ARG_NAME} PRIVATE )
        # set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/${MAC_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        
    endif()

    if(TARGET_GTK)
        add_executable(${_ARG_NAME} ${_ARG_SOURCES})
    endif()

    target_include_directories(${_ARG_NAME} PUBLIC ${_ARG_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    if(NOT TARGET_MACOS)
        target_link_libraries(${_ARG_NAME} PRIVATE ${_ARG_LINK_LIBS} "OmegaWTK")
    endif()

endfunction()

function(omega_wtk_link_to_libs LIBS_TO_LINKS)

    foreach(lib ${LIBS_TO_LINKS})
        if(TARGET ${lib})
            target_link_libraries(${lib} PRIVATE ${OMEGAWTK_LIBS})
        endif()    
    endforeach()
    
endfunction()

