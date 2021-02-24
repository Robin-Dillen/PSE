cmake_minimum_required(VERSION 3.6)

set(DEBUG_SOURCE_FILES main_test.cpp)

enable_testing()

add_executable(PSE_debug ${DEBUG_SOURCE_FILES})
target_link_libraries(PSE_debug PUBLIC gtest_main PSE_lib)

add_test(NAME PSE_debug COMMAND PSE_debug)