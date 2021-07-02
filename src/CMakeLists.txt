add_subdirectory("Core")
add_subdirectory("Native")
add_subdirectory("Media")
add_subdirectory("Composition")
add_subdirectory("UI")

# Generate Shared Library/Export
set(EXPORT_HEADER ${CMAKE_SOURCE_DIR}/include/OmegaWTK.h)

configure_file("./Info.plist.in" "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" @ONLY)
configure_file("./version.plist.in" "${CMAKE_CURRENT_BINARY_DIR}/version.plist" @ONLY)

if(TARGET_MACOS)

add_framework_bundle(
    "OmegaWTK"
    VERSION 
    ${OMEGAWTK_VERSION}
    INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
    RESOURCES "${CMAKE_CURRENT_BINARY_DIR}/version.plist"
    DEPS 
    "omegaWTKCore"
    "omegaWTKMedia"
    "omegaWTKNative"
    "omegaWTKComposition"
    "omegaWTKUI"
    LIBS
    "omegaWTKCore"
    "omegaWTKMedia"
    "omegaWTKNative"
    "omegaWTKComposition"
    "omegaWTKUI"
    FRAMEWORKS 
    "OmegaGTE"
    EMBEDDED_FRAMEWORKS
    "OmegaGTE"
    SOURCES 
    OmegaWTK.cpp
    ${EXPORT_HEADER}
    $<TARGET_OBJECTS:omegaWTKCore>
    $<TARGET_OBJECTS:omegaWTKMedia>
    $<TARGET_OBJECTS:omegaWTKNative>
    $<TARGET_OBJECTS:omegaWTKComposition>
    $<TARGET_OBJECTS:omegaWTKUI>
)

set_target_properties("OmegaWTK" PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Frameworks")

target_link_libraries("OmegaWTK" PRIVATE 
"OmegaGTE"
${ICU_COMMON_LIB}
${ICU_I18N_LIB}
${PNG_LIB}
${JPEG_LIB}
${Cocoa_LIB} 
${QuartzCore_LIB}
"omega-common")

endif()

