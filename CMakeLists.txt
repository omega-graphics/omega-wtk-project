cmake_minimum_required(VERSION 3.11)

set(OMEGAWTK_VERSION "0.8")

include(OmegaGraphicsSuite)

set(LANGS CXX)

if(APPLE)
	set(LANGS ${LANGS} OBJCXX)
endif()

omega_graphics_project(OmegaWTK VERSION ${OMEGAWTK_VERSION} LANGUAGES ${LANGS})

omega_graphics_add_subdir(OmegaGTE gte)


if(WIN32)
	set(TARGET_WIN32 TRUE)
	add_compile_definitions(TARGET_WIN32)
elseif(APPLE)
	set(TARGET_MACOS TRUE)
	add_compile_definitions(TARGET_MACOS)
elseif(UNIX AND NOT APPLE)
	set(TARGET_LINUX TRUE)
	add_compile_definitions(TARGET_LINUX)
endif()

# Third Party Modules

set(THIRD_PARTY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external-libs)
set(THIRD_PARTY_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps)
set(THIRD_PARTY_INCLUDEDIRS)

add_third_party(
	CUSTOM_PROJECT
	NAME icu 
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source" 
	BINARY_DIR "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/icu"
	CONF ./configure -prefix ${THIRD_PARTY_OUTPUT_DIR}/icu
	BUILD make all
	INSTALL make install)

	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/icu/include")
	
add_third_party(
	NAME libxml2 
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/libxml2/code" 
	BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/temp/libxml2"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/libxml2"
	CMAKE_BUILD_ARGS -DLIBXML2_WITH_ICU=TRUE -DLIBXML2_WITH_ICONV=FALSE -DLIBXML2_WITH_LZMA=FALSE -DCMAKE_PREFIX_PATH=${THIRD_PARTY_OUTPUT_DIR}/icu -DCMAKE_C_FLAGS="-I${THIRD_PARTY_OUTPUT_DIR}/icu/include"
	DEPS icu)
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libxml2/include")
	set(LIBXML2_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libxml2/lib/libxml2.dylib")

add_third_party(
	NAME zlib 
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/zlib/code" 
	BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/temp/zlib"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/zlib")
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/zlib/include")
	
add_third_party(
	NAME libpng
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/libpng/code"
	BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/temp/libpng"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/libpng"
	CMAKE_BUILD_ARGS -DCMAKE_PREFIX_PATH="${THIRD_PARTY_OUTPUT_DIR}/zlib"
	DEPS zlib)
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libpng/include")

add_third_party(
	NAME libjpeg-turbo
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/libjpeg-turbo/code"
	BINARY_DIR ${THIRD_PARTY_OUTPUT_DIR}/temp/libjpeg-turbo
	INSTALL_DIR ${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo
	CMAKE_BUILD_ARGS -DCMAKE_PREFIX_PATH="${THIRD_PARTY_OUTPUT_DIR}/zlib"
	DEPS zlib)
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo/include")
	
add_third_party(
	NAME libtiff
	SOURCE_DIR ${THIRD_PARTY_SRC_DIR}/libtiff/code
	BINARY_DIR ${THIRD_PARTY_OUTPUT_DIR}/temp/libtiff
	INSTALL_DIR ${THIRD_PARTY_OUTPUT_DIR}/libtiff
	CMAKE_BUILD_ARGS -DCMAKE_PREFIX_PATH="${THIRD_PARTY_OUTPUT_DIR}/zlib,${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo"
	DEPS zlib libjpeg-turbo)
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libtiff/include")

#Omega WTK Core SubModule

file(GLOB CORE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp")
set_source_files_properties(${CORE_SRCS} PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/tools/assetc")
source_group(OmegaWTKCore ${CORE_SRCS})

#OmegaWTK Native SubModule

file(GLOB NATIVE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/*.cpp")

if(TARGET_WIN32)
	file(GLOB WIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/win/*.cpp")
	set(NATIVE_SRCS ${NATIVE_SRCS} ${WIN_SRCS})
	
elseif(TARGET_MACOS)
	file(GLOB MACOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/macos/*.mm")
	set(NATIVE_SRCS ${NATIVE_SRCS} ${MACOS_SRCS})
	
endif()

source_group(OmegaWTKNative ${NATIVE_SRCS})
set(NATIVE_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/private_include")
set_source_files_properties(${NATIVE_SRCS} PROPERTIES INCLUDE_DIRECTORIES ${NATIVE_PRIVATE_INCLUDE_DIR})

#OmegaWTK Composition SubModule

file(GLOB COMPOSITION_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/*.cpp")

if(TARGET_WIN32)
	file(GLOB WIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/dx/*.cpp")
elseif(TARGET_MACOS)
	file(GLOB MACOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/mtl/*.mm")
	set(COMPOSITION_SRCS ${COMPOSITION_SRCS} ${MACOS_SRCS})
endif()

source_group(OmegaWTKComposition ${COMPOSITION_SRCS})
set_source_files_properties(${COMPOSITION_SRCS} PROPERTIES INCLUDE_DIRECTORIES ${NATIVE_PRIVATE_INCLUDE_DIR})

#OmegaWTK UI SubModule
file(GLOB UI_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/UI/*.cpp")
source_group(OmegaWTKUI ${UI_SRCS})

#OmegaWTK Widgets Submodule
file(GLOB WIDGETS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/UI/*.cpp")
source_group(OmegaWTKWidgets ${WIDGETS_SRCS})

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in" ${CMAKE_CURRENT_BINARY_DIR}/Info.plist @ONLY)

add_omega_graphics_module("OmegaWTK" FRAMEWORK INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" VERSION ${OMEGAWTK_VERSION} HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" DEPENDS OmegaGTE.framework icu zlib libpng libjpeg-turbo libtiff libxml2 SOURCES ${CORE_SRCS} ${NATIVE_SRCS} ${COMPOSITION_SRCS} ${UI_SRCS} ${WIDGETS_SRCS})
target_include_directories("OmegaWTK" PUBLIC ${THIRD_PARTY_INCLUDEDIRS} $<TARGET_PROPERTY:OmegaCommon,INCLUDE_DIRECTORIES> )
target_link_system_frameworks("OmegaWTK" Metal Cocoa QuartzCore)
target_link_frameworks("OmegaWTK" OmegaGTE)
target_link_libraries("OmegaWTK" PUBLIC "OmegaCommon" ${LIBXML2_EXPORT})

add_subdirectory(tests)