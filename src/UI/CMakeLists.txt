add_omega_wtk_lib(
    OBJECT
    NAME 
    "UI"
    SOURCES 
    "WidgetTreeHost.cpp"
    "Widget.cpp"
    "App.cpp"
    "View.cpp"
    "ViewAnimator.cpp"
    "VideoView.cpp"
    "Menu.cpp"
    "AppWindow.cpp"
    "Text.cpp"
    "Layout.cpp"
    HEADERS 
    "Widget.h"
    "WidgetTreeHost.h"
    "App.h"
    "View.h"
    "ViewAnimator.h"
    "VideoView.h"
    "Menu.h"
    "AppWindow.h"
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