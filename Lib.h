//============================================================================
// Name        : Lib.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : contains constants
//============================================================================

#ifndef PSE_LIB_H
#define PSE_LIB_H

#include <string>
#include <map>
#include "Vaccins.h"

#define OUTPUT_FILE_LOCATION string("../Outputfiles/")
#define HAPPY_DAY_OUTPUT_FILE_LOCATION string(OUTPUT_FILE_LOCATION + "HappyDayTests/")
#define WARNING_OUTPUT_FILE_LOCATION string(OUTPUT_FILE_LOCATION + "WarningTests/")
#define DEATH_OUTPUT_FILE_LOCATION string(OUTPUT_FILE_LOCATION + "DeathTests/")
#define INPUT_FILE_LOCATION string("../XMLfiles/")
#define DEATH_TESTS_FILE_LOCATION string(INPUT_FILE_LOCATION + "DeathTests/")
#define WARNING_TESTS_FILE_LOCATION string(INPUT_FILE_LOCATION + "WarningTests/")
#define HAPPY_DAY_TESTS_FILE_LOCATION string(INPUT_FILE_LOCATION + "HappyDayTests/")
#define ERROR_FILE "../err.txt"

#endif //PSE_LIB_H
