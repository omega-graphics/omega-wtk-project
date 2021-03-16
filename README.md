# OmegaWTK (Omega-Widget Tool Kit)
 A cross-platform widget toolkit written in c++

---
###  Build/Usage


- #### With CMake
1. Install CMake (at https://cmake.org/download or via Homebrew if on macOS or other pacakge manager)
2. Configure the current folder with minimal settings.
(We recommend that you use Ninja as your build system)
```shell
cmake -S . -B ./build -G"Ninja" 
```
3. Build the out directory :)

Example Usage

```cmake
cmake_minimum_required(VERSION 3.10)

project(TestProject CXX)

find_package(OmegaWTK CONFIG REQUIRED)

add_omega_wtk_app(
    NAME
    "TestApplication"
    FILES 
    "test.h"
    "test.cpp"
    ENTRY
    "test.h"
    BUNDLE_ID
    "org.omegawtk.examples.cmake.testapplication"
)
```
- #### With GN (Preferred)
> Note make sure you have `cmake` in your `$PATH`
1. Install GN ( Visit https://gn.googlesource.com/gn for installation)   
2. Run 
```shell
gn gen out --args=" vcpkg_installed_dir=\"/Users/myuser/dir/to/vcpkg/installed/os\""
```
3. Then run `ninja -C ./out` to build the project
4. Use.

Example Usage

```GN
import("//mybuild/externals/omega-wtk-project/gn/OmegaWTK.gni")

omega_wtk_app("TestApplication"){
    sources = [
        "src/test.h",
        "src/test.cpp"
    ]
    app_entry = "test.h"
    out_dir = "$root_out_dir/apps"
    bundle_id = "org.omegawtk.examples.gn.testapplication""
}

```