# Install script for directory: C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html

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
   "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/addingtags.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/bugs.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/build.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/contrib.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/document.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/images.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/index.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/internals.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/intro.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/libtiff.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/misc.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/support.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/TIFFTechNote2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/tools.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta007.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta016.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta018.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta024.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta028.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta029.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta031.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta032.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta033.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta034.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta035.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.4beta036.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.3.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.4.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.5.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.6-beta.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.5.7.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.6.0.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.6.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.0alpha.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.0beta.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.0beta2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.0.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.3.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.7.4.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.8.0.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.8.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.8.2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.9.0beta.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.9.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v3.9.2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.0.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.1.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.2.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.3.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.4.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.4beta.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.5.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.6.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.7.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.8.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.9.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.0.10.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.1.0.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.2.0.html;C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html/v4.3.0.html")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/share/doc/tiff/html" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/addingtags.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/bugs.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/build.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/contrib.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/document.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/images.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/index.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/internals.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/intro.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/libtiff.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/misc.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/support.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/TIFFTechNote2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/tools.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta007.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta016.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta018.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta024.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta028.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta029.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta031.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta032.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta033.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta034.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta035.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.4beta036.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.3.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.4.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.5.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.6-beta.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.5.7.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.6.0.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.6.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.0alpha.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.0beta.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.0beta2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.0.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.3.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.7.4.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.8.0.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.8.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.8.2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.9.0beta.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.9.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v3.9.2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.0.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.1.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.2.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.3.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.4.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.4beta.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.5.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.6.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.7.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.8.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.9.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.0.10.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.1.0.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.2.0.html"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code/html/v4.3.0.html"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/html/images/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/html/man/cmake_install.cmake")

endif()

