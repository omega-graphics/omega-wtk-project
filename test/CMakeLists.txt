set_source_files_properties("pch.cpp" COMPILE_FLAGS "/Ycpch.h")
set(TEST_SOURCES "Main.cpp" "pch.h")
set_source_files_properties(${TEST_SOURCES} COMPILE_FLAGS "/Yupch.h")
add_executable(
    "omega-test" 
    WIN32 
    "pch.cpp"
    ${TEST_SOURCES}
    "test.rc")
