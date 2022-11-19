cmake_minimum_required(VERSION 3.23)
project(Marlin)

set(CMAKE_CXX_STANDARD 23)

add_executable(Marlin main.cpp src/engine/types.h)
add_subdirectory(Google_test src)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# defines test build configuration linking gtest with engine
# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
include_directories(src)
include_directories(src/engine)
add_executable(Google_Tests_run Google_test/test1.cpp Google_test/tests2.cpp src/engine/types.h)
target_link_libraries(Google_Tests_run gtest gtest_main)