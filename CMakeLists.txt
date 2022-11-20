cmake_minimum_required(VERSION 3.23)
project(Marlin)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(Google_test src)

# defines main build configuration
add_executable(Marlin main.cpp src/engine/types.h src/engine/precalculated_data.cpp src/engine/precalculated_data.h)


# defines test build configuration linking gtest with engine
# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(src)
include_directories(src/engine)
add_executable(Google_Tests_run
        Google_test/precalculated_data_test.cpp
        src/engine/types.h
        src/engine/precalculated_data.h
        src/engine/precalculated_data.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)