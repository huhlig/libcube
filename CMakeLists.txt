cmake_minimum_required(VERSION 3.7)
project(ledcube CXX)

set(CMAKE_CXX_STANDARD 14)
set(WIRINGPI_ROOT_DIR C:/Users/huhlig/Projects/wiringPi/wiringPi)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED)

include_directories(${WIRINGPI_INCLUDE_DIRS})

add_executable(cube source/cube.hpp source/test.cpp source/patterns.hpp)