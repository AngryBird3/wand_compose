cmake_minimum_required(VERSION 3.31)
project(pipeline)

set(CMAKE_CXX_STANDARD 20)

add_executable(pipeline main.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(steps_util_test steps_util_test_no_short_circuit.cpp)
target_link_libraries(steps_util_test gtest_main)
add_test(NAME steps_util_test COMMAND steps_util_test)
