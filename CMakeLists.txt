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
file(GLOB server_SOURCES
        "src/server/*.h"
        "src/server/*.cpp"
        )

# defines main build configuration
add_executable(Marlin main.cpp ${engine_SOURCES})

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
        ${engine_SOURCES}
        src/testers/hvh_main.cpp
        src/testers/console_hvh_tester.cpp
        src/testers/console_hvh_tester.h
        src/testers/tester_utility.cpp
        src/testers/tester_utility.h
        )

# defines testers configuration for human vs computer
add_executable(HvC_tester
        ${engine_SOURCES}
        src/testers/hvc_main.cpp
        src/testers/console_hvc_tester.cpp
        src/testers/console_hvc_tester.h
        src/testers/tester_utility.cpp
        src/testers/tester_utility.h
        )

# defines testers configuration for computer vs computer
add_executable(CvC_tester
        ${engine_SOURCES}
        src/testers/cvc_main.cpp
        src/testers/console_cvc_tester.cpp
        src/testers/console_cvc_tester.h
        src/testers/tester_utility.cpp
        src/testers/tester_utility.h
        )

# defines configuration for server
add_executable(server
        ${engine_SOURCES}
        ${server_SOURCES}
        )
target_link_libraries(server PRIVATE ws2_32)

# defines server test configuration
add_executable(server_test
        src/testers/server_tester_main.cpp
        )
target_link_libraries(server_test PRIVATE ws2_32)