cmake_minimum_required(VERSION 3.6)
project(PSE)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        main.cpp
        Parser.cpp Parser.h
        Hub.cpp Hub.h
        VaccinatieCentrum.cpp VaccinatieCentrum.h
        TinyXML/tinystr.cpp
        TinyXML/tinystr.h
        TinyXML/tinyxml.cpp
        TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp
        TinyXML/tinyxmlparser.cpp
        Output.cpp
        Output.h)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES main_test.cpp VaccinSimulatieTests.cpp)

# Create RELEASE target
add_executable(PSE ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(PSE_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(PSE_debug gtest)