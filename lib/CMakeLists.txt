add_subdirectory("Core")
add_subdirectory("Native")
add_subdirectory("Composition")
add_subdirectory("UI")

# Generate Shared Library/Export
set(EXPORT_HEADER "${CMAKE_SOURCE_DIR}/include/OmegaWTK.h")
set(OMEGA_TARGET_SOURCES "")
foreach(LIB ${OmegaWTKLibs})
    string(APPEND OMEGA_TARGET_SOURCES "  $<TARGET_OBJECTS:${LIB}>")
endforeach()
message("OBJECTS:${OMEGA_TARGET_SOURCES}")
add_library("OmegaWTK" STATIC ${EXPORT_HEADER} "OmegaWTK.cpp" $<TARGET_OBJECTS:omegaWTKCore> $<TARGET_OBJECTS:omegaWTKNative> $<TARGET_OBJECTS:omegaWTKComposition> $<TARGET_OBJECTS:omegaWTKUI>)
if(TARGET_WIN32)
    set(WINDOWS_LINK_F "/WHOLEARCHIVE:")
    set(OMEGALIBS ${OmegaWTKLibs})
    list(TRANSFORM OMEGALIBS PREPEND "lib/")
    list(TRANSFORM OMEGALIBS APPEND ".lib")
    string(JOIN "  " OMEGA_OUT ${OMEGALIBS})
    set(WINDOWS_LINK_F "${WINDOWS_LINK_F}${OMEGA_OUT}")
    message("WindowsLIBS:${WINDOWS_LINK_F}")

    set_target_properties("OmegaWTK" PROPERTIES 
    VERSION ${OMEGAWTK_VERSION}
    INTERFACE_${_ARG_NAME}_MAJOR_VERSION ${OMEGAWTK_VERSION}
        LINK_FLAGS ${WINDOWS_LINK_F})
endif()
if(TARGET_MACOS)
    set_target_properties("OmegaWTK" PROPERTIES 
    VERSION ${OMEGAWTK_VERSION}
     INTERFACE_${_ARG_NAME}_MAJOR_VERSION ${OMEGAWTK_VERSION}
        LINK_FLAGS "-Wl,-all_load")
endif()
target_link_libraries("OmegaWTK" PUBLIC ${OmegaWTKLibs})
set(OmegaWTKLibs ${OmegaWTKLibs} "OmegaWTK" CACHE INTERNAL "OmegaWTK Libs")
target_include_directories("OmegaWTK" PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)


