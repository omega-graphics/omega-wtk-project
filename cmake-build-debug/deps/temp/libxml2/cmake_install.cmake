# Install script for directory: C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/libxml2")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml2/libxml" TYPE FILE FILES
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/c14n.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/catalog.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/chvalid.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/debugXML.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/dict.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/DOCBparser.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/encoding.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/entities.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/globals.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/hash.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/HTMLparser.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/HTMLtree.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/list.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/nanoftp.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/nanohttp.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/parser.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/parserInternals.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/pattern.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/relaxng.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/SAX.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/SAX2.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/schemasInternals.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/schematron.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/threads.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/tree.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/uri.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/valid.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xinclude.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xlink.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlIO.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlautomata.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlerror.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlexports.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlmemory.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlmodule.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlreader.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlregexp.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlsave.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlschemas.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlschemastypes.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlstring.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlunicode.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xmlwriter.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xpath.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xpathInternals.h"
    "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/include/libxml/xpointer.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml2.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xruntimex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml2.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdebugx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg]|[Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml2.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg]|[Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xprogramsx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/xmlcatalog.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xprogramsx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/xmllint.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man3" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/libxml.3")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/doc/xmlcatalog.1")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/doc/xmllint.1")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libxml2" TYPE DIRECTORY FILES "C:/Users/alex/Documents/GitHub/wtk/external-libs/libxml2/code/doc/" REGEX "/makefile\\.[^/]*$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml2-config.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml2-config-version.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12/libxml2-export.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12/libxml2-export.cmake"
         "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/CMakeFiles/Export/lib/cmake/libxml2-2.9.12/libxml2-export.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12/libxml2-export-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12/libxml2-export.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/CMakeFiles/Export/lib/cmake/libxml2-2.9.12/libxml2-export.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libxml2-2.9.12" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/CMakeFiles/Export/lib/cmake/libxml2-2.9.12/libxml2-export-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml2/libxml" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml/xmlversion.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libxml2/libxml" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml/xmlwin32version.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/libxml-2.0.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/xml2-config")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/xml2Conf.sh")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/alex/Documents/GitHub/wtk/cmake-build-debug/deps/temp/libxml2/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
