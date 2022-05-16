cmake_minimum_required(VERSION 3.10)
set(default_build_type "Release")
set(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_BUILD_TOOL "ninja")
set(CMAKE_MAKE_PROGRAM "nmake")

# set the project name
project(CaretakerControl VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/) 

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(lib)

enable_testing ()
add_test (NAME MyTest COMMAND Test)