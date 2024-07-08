cmake_minimum_required(VERSION 3.11)

set(OMEGAWTK_VERSION "0.8")


project(OmegaWTK VERSION ${OMEGAWTK_VERSION} LANGUAGES C CXX)

include(OmegaGraphicsSuite)

if(APPLE)
	enable_language(OBJCXX)
endif()

omega_graphics_add_subdir(OmegaGTE gte)

set(OMEGAWTK_MOBILE FALSE)

if(WIN32)
	set(TARGET_WIN32 TRUE)
	add_compile_definitions(TARGET_WIN32)
elseif(APPLE AND NOT IOS)
	set(TARGET_MACOS TRUE)
	add_compile_definitions(TARGET_MACOS)
elseif(IOS)
	set(TARGET_IOS TRUE)
	add_compile_definitions(TARGET_IOS TARGET_MOBILE)
	set(OMEGAWTK_MOBILE TRUE)
elseif(ANDROID)
	set(TARGET_ANDROID TRUE)
	add_compile_definitions(TARGET_ANDROID TARGET_MOBILE)
	set(OMEGAWTK_MOBILE TRUE)
elseif(UNIX AND NOT APPLE)
	set(TARGET_LINUX TRUE)
	add_compile_definitions(TARGET_GTK)
endif()

if(${OMEGAWTK_MOBILE})
	add_compile_definitions(OMEGAWTK_MOBILE_API)
endif()

# Third Party Modules

set(THIRD_PARTY_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external-libs)
set(THIRD_PARTY_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps)
set(THIRD_PARTY_INCLUDEDIRS)

set(ICU_VERSION 69)

if(TARGET_WIN32)

file(MAKE_DIRECTORY  ${THIRD_PARTY_OUTPUT_DIR}/icu/bin)
file(MAKE_DIRECTORY  ${THIRD_PARTY_OUTPUT_DIR}/icu/lib)

add_third_party(
	CUSTOM_PROJECT
	NAME icu 
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source/allinone" 
	BINARY_DIR "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source/allinone"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/icu"
	CONF "" 
	BUILD msbuild allinone.sln -p:Configuration=Release
	INSTALL ${CMAKE_COMMAND} -E copy_directory "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/include" "${THIRD_PARTY_OUTPUT_DIR}/icu/include"
	&& ${CMAKE_COMMAND} -E copy "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/bin64/icuuc${ICU_VERSION}.dll" "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/bin64/icudt${ICU_VERSION}.dll" "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/bin64/icuin${ICU_VERSION}.dll" "${THIRD_PARTY_OUTPUT_DIR}/icu/bin"
	&& ${CMAKE_COMMAND} -E copy "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/lib64/icuuc.lib" "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/lib64/icudt.lib" "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/lib64/icuin.lib" "${THIRD_PARTY_OUTPUT_DIR}/icu/lib")

	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/icu/include")

	set(ICU_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/icu/lib/icuuc.lib" "${THIRD_PARTY_OUTPUT_DIR}/icu/lib/icudt.lib" "${THIRD_PARTY_OUTPUT_DIR}/icu/lib/icuin.lib")

	set(ICU_DLLS "${THIRD_PARTY_OUTPUT_DIR}/icu/bin/icuuc69.dll" "${THIRD_PARTY_OUTPUT_DIR}/icu/bin/icudt69.dll" "${THIRD_PARTY_OUTPUT_DIR}/icu/bin/icuin69.dll")
else()

if(TARGET_IOS)
	add_third_party(
		CUSTOM_PROJECT
		NAME icu_tools 
		SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/icu-69" 
		BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/intermed/icu_mac"
		INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/intermed/icu_mac"
		CONF cd "${THIRD_PARTY_OUTPUT_DIR}/intermed/icu_mac" &&  ${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source/configure -prefix ${THIRD_PARTY_OUTPUT_DIR}/icu
		BUILD make all
		INSTALL echo "ICU Cross Tools Complete")
		
	# message("SYSROOT:${CMAKE_OSX_SYSROOT}")
	add_third_party(
		CUSTOM_PROJECT
		NAME icu 
		SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/icu-69" 
		BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/intermed/icu_ios"
		INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/icu"
		CONF 
		export CFLAGS=--sysroot=${CMAKE_OSX_SYSROOT} && 
		export CXXFLAGS=--sysroot=${CMAKE_OSX_SYSROOT} &&
		${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source/configure -prefix ${THIRD_PARTY_OUTPUT_DIR}/icu --with-cross-build=${THIRD_PARTY_OUTPUT_DIR}/intermed/icu_mac
		BUILD make all
		INSTALL make install
		DEPS icu_tools)
else()
	add_third_party(
		CUSTOM_PROJECT
		NAME icu 
		SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source" 
		BINARY_DIR "${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source"
		INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/icu"
		CONF  ${THIRD_PARTY_SRC_DIR}/icu-69/code/icu4c/source/configure -prefix ${THIRD_PARTY_OUTPUT_DIR}/icu
		BUILD make all
		INSTALL make install)
endif()

	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/icu/include")

	if(TARGET_MACOS)
		set(ICU_EXPORT ${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicuuc.dylib ${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicudata.dylib ${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicui18n.dylib)
		set_library_install_name("${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicuuc.dylib" "@rpath/libicuuc.dylib")
		set_library_install_name("${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicudata.dylib" "@rpath/libicudata.dylib")
		set_library_install_name("${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicui18n.dylib" "@rpath/libicui18n.dylib")
		reset_library_dependent_name("${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicuuc.dylib" "libicudata.69.dylib" "@rpath/libicudata.dylib")
		reset_library_dependent_name("${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicui18n.dylib" "libicudata.69.dylib" "@rpath/libicudata.dylib")
		reset_library_dependent_name("${THIRD_PARTY_OUTPUT_DIR}/icu/lib/libicui18n.dylib" "libicuuc.69.dylib" "@rpath/libicuuc.dylib")
	else()
	endif()
	
endif()

set(PREFIX_PATH ${THIRD_PARTY_OUTPUT_DIR}/zlib)

add_third_party(
	NAME libxml2 
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/libxml2/code" 
	BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/temp/libxml2"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/libxml2"
	CMAKE_BUILD_ARGS -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME} -DLIBXML2_WITH_PYTHON=FALSE -DLIBXML2_WITH_ICU=FALSE -DLIBXML2_WITH_ICONV=FALSE -DLIBXML2_WITH_LZMA=FALSE -DCMAKE_PREFIX_PATH=${PREFIX_PATH} -DCMAKE_C_FLAGS="-I${THIRD_PARTY_OUTPUT_DIR}/icu/include"
	DEPS icu)
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libxml2/include/libxml2")
	if(TARGET_WIN32)
		set(LIBXML2_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libxml2/lib/libxml2.lib")

		set(LIBXML2_DLL "${THIRD_PARTY_OUTPUT_DIR}/libxml2/bin/libxml2.dll")
	elseif(TARGET_MACOS)
		set(LIBXML2_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libxml2/lib/libxml2.dylib")
		set_library_install_name(${LIBXML2_EXPORT} "@rpath/libxml2.dylib")
	endif()

add_third_party(
	NAME zlib 
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/zlib/code" 
	BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/temp/zlib"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/zlib")
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/zlib/include")
	if(TARGET_WIN32)
		set(ZLIB_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/zlib/lib/zlib.lib")

		set(ZLIB_DLL "${THIRD_PARTY_OUTPUT_DIR}/zlib/bin/zlib.dll")
	elseif(TARGET_MACOS)
		set(ZLIB_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/zlib/lib/libz.dylib")
		set_library_install_name(${ZLIB_EXPORT} "@rpath/libz.dylib")
	endif()
	
add_third_party(
	NAME libpng
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/libpng/code"
	BINARY_DIR "${THIRD_PARTY_OUTPUT_DIR}/temp/libpng"
	INSTALL_DIR "${THIRD_PARTY_OUTPUT_DIR}/libpng"
	CMAKE_BUILD_ARGS -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME} -DCMAKE_PREFIX_PATH=${THIRD_PARTY_OUTPUT_DIR}/zlib  -DCMAKE_C_FLAGS:LIST=-DPNG_ARM_NEON_OPT=0;-DPNG_ARM_NEON_IMPLEMENTATION=0
	DEPS zlib)

	if(TARGET_WIN32)
		set(LIBPNG_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libpng/lib/libpng16.lib")

		set(LIBPNG_DLL "${THIRD_PARTY_OUTPUT_DIR}/libpng/bin/libpng16.dll")
	elseif(TARGET_MACOS)
		set(LIBPNG_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libpng/lib/libpng.dylib")
		set_library_install_name(${LIBPNG_EXPORT} "@rpath/libpng.dylib")
	endif()
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libpng/include")

add_third_party(
	NAME libjpeg-turbo
	SOURCE_DIR "${THIRD_PARTY_SRC_DIR}/libjpeg-turbo/code"
	BINARY_DIR ${THIRD_PARTY_OUTPUT_DIR}/temp/libjpeg-turbo
	INSTALL_DIR ${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo
	CMAKE_BUILD_ARGS -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR} -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME} -DCMAKE_PREFIX_PATH=${THIRD_PARTY_OUTPUT_DIR}/zlib
	DEPS zlib)

	if(TARGET_WIN32)
		set(LIBJPEGTURBO_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo/lib/turbojpeg.lib")

		set(LIBJPEGTURBO_DLL "${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo/bin/turbojpeg.dll")
	elseif(TARGET_MACOS)
		set(LIBJPEGTURBO_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo/lib/libturbojpeg.dylib")
		set_library_install_name(${LIBJPEGTURBO_EXPORT} "@rpath/libturbojpeg.dylib")
	endif()
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo/include")
	
add_third_party(
	NAME libtiff
	SOURCE_DIR ${THIRD_PARTY_SRC_DIR}/libtiff/code
	BINARY_DIR ${THIRD_PARTY_OUTPUT_DIR}/temp/libtiff
	INSTALL_DIR ${THIRD_PARTY_OUTPUT_DIR}/libtiff
	CMAKE_BUILD_ARGS -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME} -DCMAKE_PREFIX_PATH=${THIRD_PARTY_OUTPUT_DIR}/zlib;${THIRD_PARTY_OUTPUT_DIR}/libjpeg-turbo
	DEPS zlib libjpeg-turbo)

	if(TARGET_WIN32)
		set(LIBTIFF_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libtiff/lib/tiff.lib")

		set(LIBTIFF_DLL "${THIRD_PARTY_OUTPUT_DIR}/libtiff/bin/tiff.dll")
	elseif(TARGET_MACOS)
		set(LIBTIFF_EXPORT "${THIRD_PARTY_OUTPUT_DIR}/libtiff/lib/libtiff.dylib")
		set_library_install_name(${LIBTIFF_EXPORT} "@rpath/libtiff.dylib")
	endif()
	
	set(THIRD_PARTY_INCLUDEDIRS ${THIRD_PARTY_INCLUDEDIRS} "${THIRD_PARTY_OUTPUT_DIR}/libtiff/include")


# For Building ThirdParty, so one can edit code through IDE with all code symbols.

add_custom_target(ThirdParty)
add_dependencies(ThirdParty icu zlib libpng libjpeg-turbo libtiff libxml2)
#Omega WTK Core SubModule

file(GLOB CORE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Core/*.cpp")
set_source_files_properties(${CORE_SRCS} PROPERTIES INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/tools/assetc")
source_group(OmegaWTKCore ${CORE_SRCS})

if(NOT ${OMEGAWTK_MOBILE})

#OmegaWTK Native SubModule

file(GLOB NATIVE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/*.cpp")

if(TARGET_WIN32)
	file(GLOB WIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/win/*.cpp")
	set(NATIVE_SRCS ${NATIVE_SRCS} ${WIN_SRCS})
	
elseif(TARGET_MACOS)
	file(GLOB MACOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/macos/*.mm")
	set(NATIVE_SRCS ${NATIVE_SRCS} ${MACOS_SRCS})
elseif(TARGET_LINUX)
	file(GLOB GTK_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/gtk/*.cpp")
	set(NATIVE_SRCS ${NATIVE_SRCS} ${GTK_SRCS})
endif()

source_group(OmegaWTKNative ${NATIVE_SRCS})
set(NATIVE_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Native/private_include")

#OmegaWTK Media SubModule

file(GLOB MEDIA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Media/*.cpp")
source_group(OmegaWTKMedia ${MEDIA_SRCS})
if(TARGET_WIN32)
	file(GLOB WIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Media/wmf/*.cpp")
	set(MEDIA_SRCS ${MEDIA_SRCS} ${WIN_SRCS})

elseif(TARGET_MACOS)
	file(GLOB MACOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Media/avf/*.mm")
	set(MEDIA_SRCS ${MEDIA_SRCS} ${MACOS_SRCS})
else()
	file(GLOB FFMPEG_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Media/ffmpeg/*.cpp")
	set(MEDIA_SRCS ${MEDIA_SRCS} ${FFMPEG_SRCS})
endif()
# Mobile Sources
else()

	file(GLOB NATIVE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Mobile/Native/*.cpp")
	if(TARGET_ANDROID)
		file(GLOB ANDROID_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Mobile/Native/android/*.cpp")
		set(NATIVE_SRCS ${NATIVE_SRCS} ${ANDROID_SRCS})
	else()
		file(GLOB IOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Mobile/Native/ios/*.mm")
		set(NATIVE_SRCS ${NATIVE_SRCS} ${IOS_SRCS})
	endif()
	source_group(OmegaWTKNative ${NATIVE_SRCS})
	set(NATIVE_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Mobile/Native/private_include")

	#OmegaWTK Media SubModule

	file(GLOB MEDIA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Media/*.cpp")
	source_group(OmegaWTKMedia ${MEDIA_SRCS})
	if(TARGET_ANDROID)
		file(GLOB ANDROID_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Mobile/Media/android/*.cpp")
		set(MEDIA_SRCS ${MEDIA_SRCS} ${ANDROID_SRCS})
	else()
		file(GLOB IOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Mobile/Media/ios/*.mm")
		set(MEDIA_SRCS ${MEDIA_SRCS} ${IOS_SRCS})
	endif()
endif()

set_source_files_properties(${NATIVE_SRCS} PROPERTIES INCLUDE_DIRECTORIES ${NATIVE_PRIVATE_INCLUDE_DIR})

#OmegaWTK Composition SubModule

file(GLOB COMPOSITION_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/*.cpp")

if(TARGET_WIN32)
	file(GLOB WIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/dx/*.cpp")
	set(COMPOSITION_SRCS ${COMPOSITION_SRCS} ${WIN_SRCS})
elseif(TARGET_MACOS)
	file(GLOB MACOS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/mtl/*.mm")
	set(COMPOSITION_SRCS ${COMPOSITION_SRCS} ${MACOS_SRCS})
else()
	file(GLOB GTK_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/Composition/backend/gtk/*.cpp")
	set(COMPOSITION_SRCS ${COMPOSITION_SRCS} ${GTK_SRCS})
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

add_omega_graphics_module("OmegaWTK" FRAMEWORK INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" VERSION ${OMEGAWTK_VERSION} HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" 
DEPENDS OmegaGTE icu zlib libpng libjpeg-turbo libtiff libxml2 
EMBEDDED_LIBS ${ICU_EXPORT} ${LIBXML2_EXPORT} ${ZLIB_EXPORT} ${LIBPNG_EXPORT} ${LIBTURBOJPEG_EXPORT} ${LIBTIFF_EXPORT} 
SOURCES ${CORE_SRCS} ${NATIVE_SRCS} ${MEDIA_SRCS} ${COMPOSITION_SRCS} ${UI_SRCS} ${WIDGETS_SRCS})
if(TARGET_ANDROID)
	add_library( app-glue
			STATIC
			${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c )

	# You need to link static libraries against your shared native library.
	target_link_libraries("OmegaWTK" PRIVATE "app-glue")
	target_include_directories("OmegaWTK" PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue)
endif()
if(TARGET_MACOS)
	set(THIRD_PARTY_EXPORTS ${ICU_EXPORT} ${LIBXML2_EXPORT} ${ZLIB_EXPORT} ${LIBPNG_EXPORT} ${LIBTURBOJPEG_EXPORT} ${LIBTIFF_EXPORT})
		add_custom_target("OmegaWTK_ThirdPartyInstallNames")
		foreach (lib ${THIRD_PARTY_EXPORTS})
			get_filename_component(LIBNAME ${lib} NAME)
			add_dependencies("OmegaWTK_ThirdPartyInstallNames" ${LIBNAME}_install_name)
		endforeach ()
		add_dependencies("OmegaWTK_ThirdPartyInstallNames" libicuuc.dylib_libicudata.69.dylib_reset_dependent_name)
		add_dependencies("OmegaWTK_ThirdPartyInstallNames" libicui18n.dylib_libicudata.69.dylib_reset_dependent_name)
		add_dependencies("OmegaWTK_ThirdPartyInstallNames" libicui18n.dylib_libicuuc.69.dylib_reset_dependent_name)


	add_dependencies("OmegaWTK" OmegaGTE.framework)
	add_dependencies("OmegaWTK" "OmegaWTK_ThirdPartyInstallNames")
elseif(TARGET_WIN32)
	set(THIRD_PARTY_DLLS ${ICU_DLLS} ${LIBXML2_DLL} ${ZLIB_DLL} ${LIBPNG_DLL} ${LIBTURBOJPEG_DLL} ${LIBTIFF_DLL})
endif()
target_include_directories("OmegaWTK" PUBLIC ${THIRD_PARTY_INCLUDEDIRS} $<TARGET_PROPERTY:OmegaCommon,INCLUDE_DIRECTORIES> )
if(TARGET_MACOS)
	target_compile_definitions("OmegaWTK" PUBLIC RUNTIME_SHADER_COMP_SUPPORT)
	target_link_system_frameworks("OmegaWTK" Metal Cocoa QuartzCore AVFoundation VideoToolbox AudioToolbox CoreMedia CoreAudio UserNotifications)
	target_link_frameworks("OmegaWTK" OmegaGTE)
else()
	target_link_libraries("OmegaWTK" PUBLIC OmegaGTE)
	target_include_directories("OmegaWTK" PUBLIC $<TARGET_PROPERTY:OmegaGTE,INCLUDE_DIRECTORIES>)
endif()
target_link_libraries("OmegaWTK" PUBLIC "OmegaCommon" ${ICU_EXPORT} ${LIBXML2_EXPORT} ${LIBPNG_EXPORT} ${LIBJPEGTURBO_EXPORT} ${LIBTIFF_EXPORT})

if(TARGET_WIN32)
	target_compile_definitions("OmegaWTK" PRIVATE WINDOWS_PRIVATE)
endif()
if(TARGET_LINUX)
	find_package(PkgConfig QUIET REQUIRED)

	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	if(NOT GTK3_FOUND)
		message("GTK 3 not found. Please install gtk3.")
	endif()

	pkg_check_modules(FFMPEG REQUIRED libavcodec libavutil libavdevice)

	if(NOT FFMPEG_FOUND)
		message("FFMPEG development files not found. Please install libavcodec, libavutil and libavdevice")
	endif()

	target_compile_options("OmegaWTK" PUBLIC ${GTK3_CFLAGS} ${FFMPEG_CFLAGS})
	target_link_directories("OmegaWTK" PUBLIC ${GTK3_LIBRARY_DIRS} ${FFMPEG_LIBRARY_DIRS})
	target_link_libraries("OmegaWTK" PUBLIC ${GTK3_LIBRARIES} ${FFMPEG_LIBRARIES})
endif()

include(cmake/OmegaWTKApp.cmake)

 add_subdirectory(tests)