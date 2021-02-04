add_omega_wtk_lib(
    STATIC
    NAME 
    "UI"
    SOURCES 
    "Widget.cpp"
    "App.cpp"
    "View.cpp"
    HEADERS 
    "Widget.h"
    "App.h"
    "View.h"
    LINK_LIBS 
    "omegaWTKCore"
    "omegaWTKNative"
    "omegaWTKComposition"
)

target_compile_definitions("omegaWTKUI" PRIVATE "WINDOWS_PRIVATE")

# if(TARGET_WIN32)
#     target_sources("omegaWTKWidgets" PRIVATE "NativeItem_Win.h" "NativeItem_Win.cpp")
# endif()

# if(TARGET_MACOS)
#     target_sources("omegaWTKWidgets" PRIVATE "NativeItem_Mac.h" "NativeItem_Mac.mm")
# endif()