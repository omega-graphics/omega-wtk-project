# add_subdirectory("RootWidget")
# # if(SWIFT_BUILD)
# #     add_subdirectory("SwiftImport")
# # endif()

OmegaWTKApp(
    NAME 
    BasicAppTest 
    BUNDLE_ID 
    "org.omegagraphics.BasicAppTest"
    SOURCES 
    BasicAppTest/main.cpp)
