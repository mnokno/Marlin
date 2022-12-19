cmake_minimum_required(VERSION 3.23)
project(Marlin)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(Google_test src)

# defines group of source files
file(GLOB engine_SOURCES
        "src/engine/*.h"
        "src/engine/*.cpp"
        )

file(GLOB gtest_SOURCES
        "Google_test/*.h"
        "Google_test/*.cpp"
        )

# defines main build configuration
add_executable(Marlin main.cpp ${engine_SOURCES} ${gtest_SOURCES})

# include directories
include_directories(src)
include_directories(src/engine)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# defines test build configuration linking gtest with engine
# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run
        ${engine_SOURCES}
        ${gtest_SOURCES}
        )

target_link_libraries(Google_Tests_run gtest gtest_main)

# defines testers configuration for human vs human
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
        src/engine/evaluation.h
        src/engine/constants.h
        src/engine/search.cpp
        src/engine/search.h
        src/testers/console_hvc_tester.cpp
        src/testers/console_hvc_tester.h src/engine/move_ordering.cpp src/engine/move_ordering.h)

# defines testers configuration for human vs computer
add_executable(HvC_tester
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
        src/testers/console_hvh_tester.cpp
        src/testers/console_hvh_tester.h
        src/testers/tester_utility.cpp
        src/testers/tester_utility.h
        src/engine/move_generator.cpp
        src/engine/move_generator.h
        src/engine/evaluation.cpp
        src/engine/evaluation.h
        src/engine/constants.h
        src/engine/search.cpp
        src/engine/search.h
        src/testers/console_hvc_tester.cpp
        src/testers/console_hvc_tester.h
        src/testers/hvc_main.cpp
        src/testers/console_cvc_tester.cpp
        src/testers/console_cvc_tester.h
        src/engine/transposition_table.h
        src/engine/transposition_table.cpp
        src/engine/move_ordering.cpp
        src/engine/move_ordering.h)

# defines testers configuration for computer vs computer
add_executable(CvC_tester
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
        src/testers/console_hvh_tester.cpp
        src/testers/console_hvh_tester.h
        src/testers/tester_utility.cpp
        src/testers/tester_utility.h
        src/engine/move_generator.cpp
        src/engine/move_generator.h
        src/engine/evaluation.cpp
        src/engine/evaluation.h
        src/engine/constants.h
        src/engine/search.cpp
        src/engine/search.h
        src/testers/console_hvc_tester.cpp
        src/testers/console_hvc_tester.h
        src/testers/console_cvc_tester.cpp
        src/testers/console_cvc_tester.h
        src/testers/cvc_main.cpp
        src/engine/transposition_table.cpp
        src/engine/transposition_table.h src/engine/move_ordering.cpp src/engine/move_ordering.h)