
cmake_minimum_required(VERSION 3.16)

project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_library(product
    src/Product.cpp
    src/Location.cpp
    src/Status.cpp 
    src/Package.cpp
    src/PackageRegistry.cpp
    src/Driver.cpp
    src/DriverAssignmentManager.cpp
)

target_include_directories(product
    PUBLIC include
)

add_executable(MyProject
    src/main.cpp

)

target_link_libraries(MyProject
    PRIVATE product
)

enable_testing()

add_executable(product_tests
    tests/test_product.cpp
    tests/test_package_registration.cpp
    tests/test_package_search.cpp
    tests/test_package_status_history.cpp
    tests/test_driver_assignment.cpp
)

target_link_libraries(product_tests
    PRIVATE
        product
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(product_tests)
