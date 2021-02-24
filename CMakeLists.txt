cmake_minimum_required(VERSION 3.6)
project(PSE)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_EXE_LINKER_FLAGS -pthread)

add_subdirectory(googletest-release-1.10.0)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES main.cpp)

# Create RELEASE target
add_executable(PSE ${RELEASE_SOURCE_FILES})

add_library(PSE_lib STATIC ${RELEASE_SOURCE_FILES})

add_subdirectory(tests)
