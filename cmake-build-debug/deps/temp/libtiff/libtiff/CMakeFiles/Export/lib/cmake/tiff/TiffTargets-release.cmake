#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TIFF::tiff" for configuration "Release"
set_property(TARGET TIFF::tiff APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TIFF::tiff PROPERTIES
  IMPORTED_IMPLIB_RELEASE "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/lib/tiff.lib"
  IMPORTED_LOCATION_RELEASE "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/bin/tiff.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS TIFF::tiff )
list(APPEND _IMPORT_CHECK_FILES_FOR_TIFF::tiff "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/lib/tiff.lib" "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/bin/tiff.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
