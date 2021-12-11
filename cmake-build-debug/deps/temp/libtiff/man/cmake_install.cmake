# Install script for directory: C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/fax2ps.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/fax2tiff.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/pal2rgb.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/ppm2tiff.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/raw2tiff.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiff2bw.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiff2pdf.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiff2ps.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiff2rgba.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffcmp.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffcp.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffcrop.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffdither.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffdump.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffgt.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffinfo.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffmedian.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffset.1;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1/tiffsplit.1")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man1" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/fax2ps.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/fax2tiff.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/pal2rgb.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/ppm2tiff.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/raw2tiff.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiff2bw.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiff2pdf.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiff2ps.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiff2rgba.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffcmp.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffcp.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffcrop.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffdither.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffdump.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffgt.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffinfo.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffmedian.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffset.1"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/tiffsplit.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/libtiff.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFbuffer.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFClose.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFcodec.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFcolor.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFDataWidth.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFError.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFieldDataType.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFieldName.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFieldPassCount.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFieldReadCount.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFieldTag.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFieldWriteCount.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFFlush.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFGetField.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFmemory.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFOpen.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFPrintDirectory.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFquery.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadDirectory.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadEncodedStrip.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadEncodedTile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadRawStrip.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadRawTile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadRGBAImage.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadRGBAStrip.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadRGBATile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadScanline.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFReadTile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFRGBAImage.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFSetDirectory.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFSetField.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFsize.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFstrip.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFswab.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFtile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWarning.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteDirectory.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteEncodedStrip.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteEncodedTile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteRawStrip.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteRawTile.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteScanline.3tiff;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3/TIFFWriteTile.3tiff")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/man/man3" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/libtiff.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFbuffer.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFClose.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFcodec.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFcolor.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFDataWidth.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFError.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFieldDataType.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFieldName.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFieldPassCount.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFieldReadCount.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFieldTag.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFieldWriteCount.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFFlush.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFGetField.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFmemory.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFOpen.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFPrintDirectory.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFquery.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadDirectory.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadEncodedStrip.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadEncodedTile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadRawStrip.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadRawTile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadRGBAImage.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadRGBAStrip.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadRGBATile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadScanline.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFReadTile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFRGBAImage.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFSetDirectory.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFSetField.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFsize.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFstrip.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFswab.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFtile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWarning.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteDirectory.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteEncodedStrip.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteEncodedTile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteRawStrip.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteRawTile.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteScanline.3tiff"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/man/TIFFWriteTile.3tiff"
    )
endif()

