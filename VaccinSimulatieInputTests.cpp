//============================================================================
// Name        : VaccinSimulatieInputTest.cpp
// Author      : Serge Demeyer
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <gtest/gtest.h>

using namespace std;

#include "Lib.h"
#include "Parser.h"
#include "Utils.h"

class VaccinSimulatieInputTest : public ::testing::Test {
protected:

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

/**
Tests InputHappyDay
*/
TEST_F(VaccinSimulatieInputTest, InputHappyDay) {
    string testnr = "001";
    int nr = 1;
    string filename = HAPPY_DAY_TESTS_FILE_LOCATION + "test" + testnr + ".xml";
    while (FileExists(filename)) {
        EXPECT_NO_FATAL_FAILURE(Parser P(filename));
        nr++;
        string new_testnr = to_string(nr);
        while (new_testnr.size() < 3) new_testnr.insert(new_testnr.begin(), '0');
        size_t pos = filename.find(testnr);
        filename.replace(pos, 3, new_testnr);
    }
}

TEST_F(VaccinSimulatieInputTest, InputXMLSyntaxErrors) {
    string testnr = "001";
    int nr = 1;
    string filename = DEATH_TESTS_FILE_LOCATION + "test" + testnr + ".xml";
    while (FileExists(filename)) {
        EXPECT_DEATH(Parser P(filename), "");
        nr++;
        string new_testnr = to_string(nr);
        while (new_testnr.size() < 3) new_testnr.insert(new_testnr.begin(), '0');
        size_t pos = filename.find(testnr);
        filename.replace(pos, 3, new_testnr);
    }
}

/*!
 * tests wether too many tags were give, or when an unknown element is read
 */
TEST_F(VaccinSimulatieInputTest, InputXMLWarnings) {
    string testnr = "001";
    int nr = 1;
    string filename = WARNING_TESTS_FILE_LOCATION + "test" + testnr + ".xml";
    while (FileExists(filename)) {
        EXPECT_DEATH(Parser P(filename), "");
        nr++;
        string new_testnr = to_string(nr);
        while (new_testnr.size() < 3) new_testnr.insert(new_testnr.begin(), '0');
        size_t pos = filename.find(testnr);
        filename.replace(pos, 3, new_testnr);
    }
}

