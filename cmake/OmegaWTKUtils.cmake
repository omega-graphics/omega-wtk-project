
include(CMakeParseArguments)

if(WIN32)
    add_compile_definitions("TARGET_WIN32")
    set(TARGET_WIN32 TRUE)
endif()

if(CMAKE_HOST_APPLE)
    if(NOT XCODE)
        set(FATAL_ERROR "Xcode Generator is Needed to Build an OmegaWTK App For Mac!")
    endif()

    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")

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
    cmake_parse_arguments("_ARG" "" "NAME;MAIN;MAC_BUNDLE_ID;WIN_ICO;MAC_ICON" "SOURCES;LINK_LIBS;INCLUDE_DIRS" ${ARGN})

    if(TARGET_WIN32)
        add_executable(${_ARG_NAME} WIN32 ${_ARG_SOURCES})
        set(OMEGAWTK_WINDOWS_UTILS_DIR  ${OMEGAWTK_TARGET_UTILS_DIR}/windows)
        set(WINDOWS_UTIL_FILES "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.rc" "${CMAKE_CURRENT_BINARY_DIR}/targetver.h" "${CMAKE_CURRENT_BINARY_DIR}/resource.h" "${CMAKE_CURRENT_BINARY_DIR}/mmain.cpp" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.exe.manifest")
        set(APPNAME ${_ARG_NAME})
        set(APPENTRY ${_ARG_MAIN})
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/mmain.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/mmain.cpp")
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/resource_script.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.rc" @ONLY)
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/resource.h.in" "${CMAKE_CURRENT_BINARY_DIR}/resource.h" @ONLY)
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/targetver.h" "${CMAKE_CURRENT_BINARY_DIR}/targetver.h" @ONLY)
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/app.ico" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.ico" COPYONLY)
        configure_file("${OMEGAWTK_WINDOWS_UTILS_DIR}/app.exe.manifest" "${CMAKE_CURRENT_BINARY_DIR}/${_ARG_NAME}.exe.manifest")
        file(COPY "${OMEGAWTK_WINDOWS_UTILS_DIR}/small.ico" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/mmain.cpp" PROPERTIES COMPILE_DEFINITIONS "WINDOWS_PRIVATE")
        target_sources(${_ARG_NAME} PRIVATE ${WINDOWS_UTIL_FILES})
    endif()

    if(TARGET_MACOS)
        add_executable(${_ARG_NAME} MACOSX_BUNDLE ${_ARG_SOURCES} "Assets.xcassets")
        set(OMEGAWTK_MACOS_UTILS_DIR ${OMEGAWTK_TARGET_UTILS_DIR}/macos)
        set(MACOS_UTIL_FILES "AppDelegate.h" "AppDelegate.mm" "English.lproj/MainMenu.xib" "main.mm" )
        list(TRANSFORM MACOS_UTIL_FILES PREPEND "${OMEGAWTK_MACOS_UTILS_DIR}/")
        foreach(_F IN ITEMS ${MACOS_UTIL_FILES})
            file(COPY ${_F} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        endforeach()
        set(APPENTRY ${_ARG_MAIN})
        set(APPNAME ${_ARG_NAME})
        configure_file( "${OMEGAWTK_MACOS_UTILS_DIR}/AppDelegate.mm" "${CMAKE_CURRENT_BINARY_DIR}/AppDelegate.mm" @ONLY)
		set(MAC_ICON ${_ARG_MAC_ICON})
		message("MAC ICON:${MAC_ICON}")
		
		configure_file("${OMEGAWTK_MACOS_UTILS_DIR}/Info.plist.in" "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" @ONLY)
		
        set(MACOS_UTILS "AppDelegate.mm" "AppDelegate.h" "MainMenu.xib" "main.mm")
        list(TRANSFORM MACOS_UTILS PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
        target_sources(${_ARG_NAME} PRIVATE ${MACOS_UTILS})
        # set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/${MAC_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        set_target_properties(${_ARG_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
            RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/MainMenu.xib;Assets.xcassets"
            MACOSX_FRAMEWORK_IDENTIFIER ${_ARG_MAC_BUNDLE_ID}
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${_ARG_MAC_BUNDLE_ID}
			XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME ${MAC_ICON}
        )
        set(_ARG_LINK_LIBS ${_ARG_LINK_LIBS} ${Cocoa_LIB})
        add_custom_target(metal-copy DEPENDS ${_ARG_NAME})
        add_custom_command(
            TARGET "metal-copy" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${METAL_LIB} $<TARGET_BUNDLE_DIR:$<TARGET_NAME_IF_EXISTS:${_ARG_NAME}>>/Contents/Resources/default.metallib)
        
    endif()

    if(TARGET_GTK)
        add_executable(${_ARG_NAME} ${_ARG_SOURCES})
    endif()

    target_include_directories(${_ARG_NAME} PUBLIC ${_ARG_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${_ARG_NAME} PRIVATE ${_ARG_LINK_LIBS} "omegaWTKNative;omegaWTKCore;omegaWTKComposition;omegaWTKUI")

endfunction()

function(omega_wtk_link_to_libs LIBS_TO_LINKS)

    foreach(lib ${LIBS_TO_LINKS})
        if(TARGET ${lib})
            target_link_libraries(${lib} PRIVATE ${OMEGAWTK_LIBS})
        endif()    
    endforeach()
    
endfunction()

