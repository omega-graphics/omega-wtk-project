add_omega_wtk_lib(
    STATIC
    NAME 
    "UI"
    SOURCES 
    "Widget.cpp"
    "App.cpp"
    "View.cpp"
    "ViewAnimator.cpp"
    "VideoView.cpp"
    "Menu.cpp"
    "AppWindow.cpp"
    "ButtonBase.cpp"
    "PushButton.cpp"
    HEADERS 
    "Widget.h"
    "App.h"
    "View.h"
    "ViewAnimator.h"
    "VideoView.h"
    "Menu.h"
    "AppWindow.h"
    "ButtonBase.h"
    "PushButton.h"
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