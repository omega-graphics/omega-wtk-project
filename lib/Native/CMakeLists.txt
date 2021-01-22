add_omega_wtk_lib(
    STATIC 
    NAME 
    "Native"
    SOURCES 
    "NativeItem.cpp"
    HEADERS
    "NativeItem.h" 
    LINK_LIBS
    "omegaWTKCore"
)

if(TARGET_WIN32)
    set(NATIVE_WIN_HEADERS "HWNDItem.h")
    set(NATIVE_WIN_SOURCES  "MakeItem.h" "MakeItem.cpp")

    list(TRANSFORM NATIVE_WIN_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/private_include/NativePrivate/win/")
    list(TRANSFORM NATIVE_WIN_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/win/")

    target_sources("omegaWTKNative" PRIVATE ${NATIVE_WIN_HEADERS} ${NATIVE_WIN_SOURCES})
endif()

if(TARGET_MACOS)
    set(NATIVE_MAC_HEADERS "CocoaItem.h")
    set(NATIVE_MAC_SOURCES "MakeItem.h" "MakeItem.mm" "CocoaItem.mm")

    list(TRANSFORM NATIVE_MAC_HEADERS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/private_include/NativePrivate/macos/")
    list(TRANSFORM NATIVE_MAC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/macos/")

    target_sources("omegaWTKNative" PRIVATE ${NATIVE_MAC_HEADERS} ${NATIVE_MAC_SOURCES})
endif()


target_include_directories("omegaWTKNative" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/private_include")