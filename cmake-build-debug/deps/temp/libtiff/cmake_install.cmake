# Install script for directory: C:/Users/alex/Documents/GitHub/wtk/external-libs/libtiff/code

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
   "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/lib/pkgconfig/libtiff-4.pc")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libtiff/lib/pkgconfig" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/libtiff-4.pc")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/port/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/libtiff/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/tools/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/test/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/contrib/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/build/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/man/cmake_install.cmake")
  include("C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/html/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libtiff/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
