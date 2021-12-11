# Install script for directory: C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man

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
   "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/index.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/libtiff.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFbuffer.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFClose.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFcodec.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFcolor.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFDataWidth.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFError.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFieldDataType.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFieldName.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFieldPassCount.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFieldReadCount.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFieldTag.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFieldWriteCount.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFFlush.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFGetField.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFmemory.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFOpen.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFPrintDirectory.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFquery.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadDirectory.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadEncodedStrip.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadEncodedTile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadRawStrip.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadRawTile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadRGBAImage.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadRGBAStrip.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadRGBATile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadScanline.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFReadTile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFRGBAImage.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFSetDirectory.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFSetField.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFsize.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFstrip.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFswab.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFtile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWarning.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteDirectory.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteEncodedStrip.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteEncodedTile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteRawStrip.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteRawTile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteScanline.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/TIFFWriteTile.3tiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/fax2ps.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/fax2tiff.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/pal2rgb.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/ppm2tiff.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/raw2tiff.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiff2bw.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiff2pdf.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiff2ps.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiff2rgba.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffcmp.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffcp.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffcrop.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffdither.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffdump.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffgt.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffinfo.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffmedian.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffset.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man/tiffsplit.1.html")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/man" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/index.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/libtiff.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFbuffer.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFClose.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFcodec.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFcolor.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFDataWidth.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFError.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFieldDataType.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFieldName.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFieldPassCount.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFieldReadCount.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFieldTag.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFieldWriteCount.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFFlush.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFGetField.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFmemory.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFOpen.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFPrintDirectory.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFquery.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadDirectory.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadEncodedStrip.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadEncodedTile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadRawStrip.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadRawTile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadRGBAImage.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadRGBAStrip.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadRGBATile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadScanline.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFReadTile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFRGBAImage.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFSetDirectory.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFSetField.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFsize.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFstrip.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFswab.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFtile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWarning.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteDirectory.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteEncodedStrip.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteEncodedTile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteRawStrip.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteRawTile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteScanline.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/TIFFWriteTile.3tiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/fax2ps.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/fax2tiff.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/pal2rgb.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/ppm2tiff.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/raw2tiff.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiff2bw.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiff2pdf.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiff2ps.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiff2rgba.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffcmp.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffcp.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffcrop.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffdither.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffdump.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffgt.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffinfo.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffmedian.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffset.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/man/tiffsplit.1.html"
    )
endif()

