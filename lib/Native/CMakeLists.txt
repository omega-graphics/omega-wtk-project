add_omega_wtk_lib(
    STATIC
    NAME 
    "Native"
    SOURCES 
    "NativeItem.cpp"
    "NativeApp.cpp"
    "NativeEvent.cpp"
    HEADERS
    "NativeItem.h" 
    "NativeApp.h"
    "NativeEvent.h"
    LINK_LIBS
    "omegaWTKCore"
)

if(TARGET_WIN32)
    set(NATIVE_WIN_HEADERS "HWNDItem.h" "WinEvent.h")
    set(NATIVE_WIN_SOURCES  "MakeItem.h" "MakeItem.cpp" "HWNDItem.cpp" "HWNDFactory.h" "HWNDFactory.cpp"  "WinMenu.h" "WinMenu.cpp" "WinEvent.cpp")

    list(TRANSFORM NATIVE_WIN_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/private_include/NativePrivate/win/")
    list(TRANSFORM NATIVE_WIN_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/win/")

    target_sources("omegaWTKNative" PRIVATE ${NATIVE_WIN_HEADERS} ${NATIVE_WIN_SOURCES})
    target_compile_definitions("omegaWTKNative" PRIVATE "WINDOWS_PRIVATE")
endif()

if(TARGET_MACOS)
    set(NATIVE_MAC_HEADERS "CocoaItem.h" "CocoaEvent.h" "CocoaUtils.h")
    set(NATIVE_MAC_SOURCES "MakeItem.h" "MakeItem.mm" "CocoaItem.mm" "CocoaEvent.mm" "CocoaUtils.mm" "CocoaApp.h" "CocoaApp.mm")

    set_source_files_properties(${NATIVE_MAC_HEADERS} ${NATIVE_MAC_SOURCES} PROPERTIES LANGUAGE OBJCXX)

    list(TRANSFORM NATIVE_MAC_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/private_include/NativePrivate/macos/")
    list(TRANSFORM NATIVE_MAC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/macos/")

    target_sources("omegaWTKNative" PRIVATE ${NATIVE_MAC_HEADERS} ${NATIVE_MAC_SOURCES})
    target_link_libraries("omegaWTKNative" PRIVATE ${Cocoa_LIB} ${QuartzCore_LIB})
endif()


target_include_directories("omegaWTKNative" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private_include")