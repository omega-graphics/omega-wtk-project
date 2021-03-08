add_omega_wtk_lib(
    STATIC
    NAME 
    "Core"
    SOURCES 
    "Core.cpp"
    HEADERS
    "Core.h"
    "Math.h"
)

if(TARGET_MACOS)
    target_sources("omegaWTKCore" PRIVATE "FSCocoa.h" "FSCocoa.mm")
    target_link_libraries("omegaWTKCore" PRIVATE ${Cocoa_LIB})
endif()