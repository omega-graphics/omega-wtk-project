add_subdirectory("Core")
add_subdirectory("Native")
add_subdirectory("Composition")
add_subdirectory("UI")

# Generate Shared Library/Export
set(EXPORT_HEADER "${CMAKE_SOURCE_DIR}/include/OmegaWTK.h")
add_library("OmegaWTK" SHARED ${EXPORT_HEADER} "OmegaWTK.cpp")
if(TARGET_WIN32)
    set_target_properties("OmegaWTK" PROPERTIES 
    VERSION ${OMEGAWTK_VERSION}
    INTERFACE_${_ARG_NAME}_MAJOR_VERSION ${OMEGAWTK_VERSION}
        LINK_FLAGS "/WHOLEARCHIVE")
endif()
if(TARGET_MACOS)
    set_target_properties("OmegaWTK" PROPERTIES 
    VERSION ${OMEGAWTK_VERSION}
     INTERFACE_${_ARG_NAME}_MAJOR_VERSION ${OMEGAWTK_VERSION}
        LINK_FLAGS "-Wl,-all_load")
endif()
target_link_libraries("OmegaWTK" PUBLIC ${OmegaWTKLibs})
set(OmegaWTKLibs ${OmegaWTKLibs} "OmegaWTK" CACHE INTERNAL "OmegaWTK Libs")


