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
    "Panel.cpp"
    "DropDown.cpp"
    "AppWindow.cpp"
    "ButtonBase.cpp"
    "PushButton.cpp"
    "Stack.cpp"
    "Text.cpp"
    "Layout.cpp"
    HEADERS 
    "Widget.h"
    "App.h"
    "Panel.h"
    "DropDown.h"
    "View.h"
    "ViewAnimator.h"
    "VideoView.h"
    "Menu.h"
    "AppWindow.h"
    "ButtonBase.h"
    "PushButton.h"
    "Stack.h"
    "Text.h"
    "Layout.h"
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