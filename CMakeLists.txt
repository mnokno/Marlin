cmake_minimum_required(VERSION 3.23)
project(Marlin)

set(CMAKE_CXX_STANDARD 23)

add_executable(Marlin main.cpp src/engine/types.h)
add_subdirectory(Google_test src)