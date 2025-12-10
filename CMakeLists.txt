cmake_minimum_required(VERSION 4.1)

project(RayTracerChallenge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Path to Catch2 relative to project1
# add_subdirectory("../packages/catch2" catch2_build)

find_package(Catch2 3 REQUIRED CONFIG)
enable_testing()
# These tests can use the Catch2-provided main
add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# These tests need their own main
# add_executable(custom-main-tests test.cpp test-main.cpp)
# target_link_libraries(custom-main-tests PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tests)