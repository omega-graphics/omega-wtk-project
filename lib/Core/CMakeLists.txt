add_omega_wtk_lib(
    STATIC
    NAME 
    "Core"
    SOURCES 
    "Core.cpp"
    "MultiThreading.cpp"
    HEADERS
    "Core.h"
    "Math.h"
    "MultiThreading.h"
)

if(TARGET_MACOS)
    target_sources("omegaWTKCore" PRIVATE "FSCocoa.h" "FSCocoa.mm")
    target_link_libraries("omegaWTKCore" PRIVATE ${Cocoa_LIB})
endif()