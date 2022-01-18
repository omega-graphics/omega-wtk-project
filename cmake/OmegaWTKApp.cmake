# OmegaWTK App Build Function

function(OmegaWTKApp)

    cmake_parse_arguments("_ARG" "" "NAME;BUNDLE_ID;ASSET_DIR;BUNDLE_ICON" "SOURCES;DEPS" ${ARGN})
    
    if(TARGET_MACOS)
        set(BUNDLE_ID ${_ARG_BUNDLE_ID})
        set(APPNAME ${_ARG_NAME})
        set(BUNDLE_ICON ${_ARG_BUNDLE_ICON})
        configure_file(${CMAKE_SOURCE_DIR}/target/macos/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist @ONLY)
        add_app_bundle(NAME ${_ARG_NAME} PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" RESOURCES ${_ARG_BUNDLE_ICON} DEPS OmegaWTK.framework OmegaGTE.framework ${_ARG_DEPS} EMBEDDED_FRAMEWORKS OmegaWTK OmegaGTE SOURCES ${_ARG_SOURCES})
        add_dependencies(${_ARG_NAME} OmegaWTK.framework)
        target_link_frameworks(${_ARG_NAME} OmegaGTE OmegaWTK)
        target_link_options(${_ARG_NAME} PRIVATE -rpath @loader_path/../Frameworks/OmegaWTK.framework/Libraries)
    else()
        add_executable(${_ARG_NAME} ${_ARG_SOURCES})
        add_dependencies(${_ARG_NAME} "OmegaWTK")
    endif()
    if(TARGET_MACOS)
        target_include_directories(${_ARG_NAME} PRIVATE "include" "gte/include" "gte/common/include" "${CMAKE_BINARY_DIR}/deps/icu/include")
        target_compile_definitions(${_ARG_NAME} PRIVATE TARGET_MACOS TARGET_METAL)
        target_sources(${_ARG_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/target/macos/main.mm)
    elseif(TARGET_WIN32)
        target_include_directories(${_ARG_NAME} PRIVATE "include" "gte/include" "gte/common/include" "${CMAKE_BINARY_DIR}/deps/icu/include")
        target_compile_definitions(${_ARG_NAME} PRIVATE WINDOWS_PRIVATE TARGET_WIN32 TARGET_DIRECTX)
        set(EMBED ${CMAKE_SOURCE_DIR}/target/windows/app.exe.manifest)
        configure_file(${CMAKE_SOURCE_DIR}/target/windows/manifest.rc.in ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
        target_sources(${_ARG_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/target/windows/mmain.cpp ${CMAKE_CURRENT_BINARY_DIR}/res.rc)
        target_link_libraries(${_ARG_NAME} PUBLIC OmegaWTK)
        target_link_options(${_ARG_NAME} PRIVATE /SUBSYSTEM:CONSOLE /ENTRY:WinMainCRTStartup /MANIFEST:NO)

        set(THIRD_PARTY_COPY "")
        foreach(lib ${THIRD_PARTY_DLLS})
            get_filename_component(FNAME ${lib} NAME)
            set(THIRD_PARTY_COPY ${THIRD_PARTY_COPY} ${CMAKE_COMMAND} -E copy ${lib} ${CMAKE_CURRENT_BINARY_DIR}/${FNAME} &&)
        endforeach()


        add_custom_command(TARGET 
                            ${_ARG_NAME}
                            POST_BUILD 
                            COMMAND 
                            ${THIRD_PARTY_COPY} ${CMAKE_COMMAND} -E copy $<TARGET_FILE:OmegaCommon> ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:OmegaCommon>
                            COMMAND 
                            ${CMAKE_COMMAND} -E copy $<TARGET_FILE:OmegaGTE> ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:OmegaGTE>
                            COMMAND 
                            ${CMAKE_COMMAND} -E copy $<TARGET_FILE:OmegaWTK> ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:OmegaWTK>
                            COMMAND)
    endif() 
endfunction()


