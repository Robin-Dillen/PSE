cmake_minimum_required(VERSION 3.17)
project(PSE)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(googletest)
include_directories(googletest/googletest/include)
include_directories(googletest/googlemock/include)

add_executable(PSE main.cpp tests/test.cpp)
target_link_libraries(PSE gtest gtest_main)