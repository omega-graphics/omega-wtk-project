add_omega_wtk_lib(
    STATIC
    NAME 
    "Widgets"
    SOURCES 
    "Widget.cpp"
    "Label.cpp"
    "Button.cpp"
    HEADERS 
    "Widget.h"
    "Label.h"
    "Button.h"
    LINK_LIBS 
    "omegaWTKCore"
)

if(TARGET_WIN32)
    target_sources("omegaWTKWidgets" PRIVATE "NativeItem_Win.h" "NativeItem_Win.cpp")
endif()

if(TARGET_MACOS)
    target_sources("omegaWTKWidgets" PRIVATE "NativeItem_Mac.h" "NativeItem_Mac.mm")
endif()