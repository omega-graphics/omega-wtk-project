# Install script for directory: C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libjpeg-turbo")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/turbojpeg.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/turbojpeg.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/tjbench.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE OPTIONAL FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/turbojpeg.pdb")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/turbojpeg-static.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/turbojpeg.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/jpeg-static.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/rdjpgcom.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/wrjpgcom.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libjpeg-turbo" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/README.ijg"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/README.md"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/example.txt"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/tjexample.c"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/libjpeg.txt"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/structure.txt"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/usage.txt"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/wizard.txt"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/LICENSE.md"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/pkgscripts/libjpeg.pc"
    "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/pkgscripts/libturbojpeg.pc"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/jconfig.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/jerror.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/jmorecfg.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libjpeg-turbo/code/jpeglib.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/simd/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/sharedlib/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/md5/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libjpeg-turbo/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
