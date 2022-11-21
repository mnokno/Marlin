cmake_minimum_required(VERSION 3.23)
project(Marlin)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(Google_test src)

# defines main build configuration
add_executable(Marlin main.cpp src/engine/types.h src/engine/precalculated_data.cpp src/engine/precalculated_data.h src/engine/bitops.cpp src/engine/bitops.h src/engine/position.cpp src/engine/position.h src/engine/prng.cpp src/engine/prng.h src/engine/zobrist_hashing.cpp src/engine/zobrist_hashing.h src/engine/move_generator.cpp src/engine/move_generator.h src/engine/evaluation.cpp src/engine/evaluation.h src/engine/constants.h)

# include directories
include_directories(src)
include_directories(src/engine)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# defines test build configuration linking gtest with engine
# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run
        Google_test/precalculated_data_test.cpp
        src/engine/types.h
        src/engine/precalculated_data.h
        src/engine/precalculated_data.cpp
        src/engine/bitops.h
        src/engine/bitops.cpp
        Google_test/bitops_test.cpp
        src/engine/position.cpp
        src/engine/position.h
        src/engine/prng.cpp
        src/engine/prng.h
        Google_test/prng_test.cpp
        src/engine/zobrist_hashing.cpp
        src/engine/zobrist_hashing.h
        Google_test/zobrist_hashing_test.cpp
        Google_test/position.cpp
        src/engine/move_generator.cpp
        src/engine/move_generator.h
        Google_test/move_generator_test.cpp
        src/engine/evaluation.cpp
        src/engine/evaluation.h src/engine/constants.h)
target_link_libraries(Google_Tests_run gtest gtest_main)

# defines testers configuration
add_executable(HvH_tester
        src/engine/types.h
        src/engine/precalculated_data.h
        src/engine/precalculated_data.cpp
        src/engine/bitops.h
        src/engine/bitops.cpp
        src/engine/position.cpp
        src/engine/position.h
        src/engine/prng.cpp
        src/engine/prng.h
        src/engine/zobrist_hashing.cpp
        src/engine/zobrist_hashing.h
        src/testers/hvh_main.cpp
        src/testers/console_hvh_tester.cpp
        src/testers/console_hvh_tester.h
        src/testers/tester_utility.cpp
        src/testers/tester_utility.h
        src/engine/move_generator.cpp
        src/engine/move_generator.h
        src/engine/evaluation.cpp
        src/engine/evaluation.h src/engine/constants.h)