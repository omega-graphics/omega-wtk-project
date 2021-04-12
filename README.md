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
    "test.cpp"
    BUNDLE_ID
    "org.omegagraphics.omegawtk.examples.testapplication"
)
```
- #### With GN (Preferred)
##### NOTE: Make sure you have `cmake` in your `$PATH`
1. Install GN ( Visit https://gn.googlesource.com/gn for installation)   
2. Run (Downloads dependencies)
```shell
python deps.py
```
3. Then run GN
```shell
gn gen out --args=""
```
3. Lastly run `ninja -C ./out` to build the project
4. Use.

Example Usage

```GN
import("//mybuild/externals/omega-wtk-project/gn/EXPORT.gni")
OmegaWTK_Export(){}


omega_wtk_app("TestApplication"){
    sources = [
        "src/test.cpp"
    ]
    out_dir = "$root_out_dir/apps"
    bundle_id = "org.omegagraphics.omegawtk.examples.testapplication""
}

```
