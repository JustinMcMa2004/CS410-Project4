/* Placeholder for the project's CMake configuration. */
/* Implementation only for spring 0 -> */

#if 0
cmake_minimum_required(VERSION 3.16)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyProject
    src/main.cpp
    src/Product.cpp
)

target_include_directories(MyProject
    PRIVATE include
)
#endif
