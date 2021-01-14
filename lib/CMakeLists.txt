set(_HEADERS "pch.h" "CoreGraphics.h")
list(TRANSFORM _HEADERS PREPEND "{CMAKE_SOURCE_DIR}/include/omegaWTK/")
set_source_files_properties("pch.cpp" COMPILE_FLAGS "/YcomegaWTK/pch.h")
set(_SOURCES "CoreGraphics.cpp")
set_source_files_properties(${_SOURCES} COMPILE_FLAGS "/YuomegaWTK/pch.h")
add_library("omegaWTKCore"
    STATIC 
    ${HEADERS}
    "pch.cpp"
    ${_SOURCES}
)