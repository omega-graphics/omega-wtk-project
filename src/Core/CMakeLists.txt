add_omega_wtk_lib(
    OBJECT 
    NAME 
    "Core"
    SOURCES 
    "Core.cpp"
    "MultiThreading.cpp"
    "AssetsPriv.h"
    "Unicode.cpp"
    HEADERS
    "Core.h"
    "MultiThreading.h"
    "Unicode.h"
)

# All Third Party Libraries must be built and installed before building the first module.
add_dependencies("omegaWTKCore" "ICU")

target_include_directories("omegaWTKCore" PRIVATE "../../tools/assetc")