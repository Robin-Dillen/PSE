cmake_minimum_required(VERSION 3.6)
project(PSE)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

IF (WIN32)
    # QT
    set(Qt5Core_DIR "\\mnt\\c\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Core")
    set(Qt5Widgets_DIR "\\mnt\\c\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Widgets")
    set(Qt5Gui_DIR "\\mnt\\c\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Gui")
else ()
    IF (EXISTS /home/niels)
        set(Qt5Core_DIR "/home/niels/Qt5.12.10/5.12.10/gcc_64/lib/cmake/Qt5Core")
        set(Qt5Widgets_DIR "/home/niels/Qt5.12.10/5.12.10/gcc_64/lib/cmake/Qt5Widgets")
        set(Qt5Gui_DIR "/home/niels/Qt5.12.10/5.12.10/gcc_64/lib/cmake/Qt5Gui")
    ELSEIF (EXISTS /home/robin)
        set(Qt5Core_DIR "/home/robin/Qt5.12.10/5.12.10/gcc_64/lib/cmake/Qt5Core")
        set(Qt5Widgets_DIR "/home/robin/Qt5.12.10/5.12.10/gcc_64/lib/cmake/Qt5Widgets")
        set(Qt5Gui_DIR "/home/robin/Qt5.12.10/5.12.10/gcc_64/lib/cmake/Qt5Gui")
    ENDIF ()
endif ()

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        main.cpp
        Parser.cpp Parser.h
        Hub.cpp Hub.h
        VaccinatieCentrum.cpp VaccinatieCentrum.h
        TinyXML/tinystr.cpp TinyXML/tinystr.h
        TinyXML/tinyxml.cpp TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp
        TinyXML/tinyxmlparser.cpp
        Output.cpp Output.h
        Lib.h
        Utils.h
        VaccinSimulatie.h
        StatisticsSingleton.cpp StatisticsSingleton.h
        Vaccins.cpp Vaccins.h
        )





# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES
        VaccinSimulatieTests.cpp
        Parser.cpp Parser.h
        Hub.cpp Hub.h
        VaccinatieCentrum.cpp VaccinatieCentrum.h
        TinyXML/tinystr.cpp TinyXML/tinystr.h
        TinyXML/tinyxml.cpp TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp
        TinyXML/tinyxmlparser.cpp
        Output.cpp Output.h
        Lib.h
        Utils.h
        VaccinSimulatie.h
        StatisticsSingleton.cpp StatisticsSingleton.h
        Vaccins.cpp Vaccins.h
        VaccinSimulatieOutputTests.cpp
        VaccinSimulatieInputTests.cpp
        VaccinSimulatieDomainTests.cpp)


# Create RELEASE target
add_executable(PSE ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(PSE_debug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(PSE_debug gtest)

qt5_use_modules(PSE Core Widgets Gui)
