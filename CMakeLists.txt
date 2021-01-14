cmake_minimum_required(VERSION 3.13)

project("OmegaWTK" C CXX)

include_directories("include")

add_subdirectory("lib")
add_subdirectory("test")