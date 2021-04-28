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
        freopen(ERROR_FILE, "a+", stderr);
    }

    virtual void TearDown() {
        fclose(stderr);
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
TEST_F(VaccinSimulatieInputTest, InputXMLWarning001) {
    Parser P("../XMLfiles/WarningTests/test001.xml");
    EXPECT_EQ(3, P.errorOccured(UNKNOWN_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning002) {
    Parser P("../XMLfiles/WarningTests/test002.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning003) {
    Parser P("../XMLfiles/WarningTests/test003.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning004) {
    Parser P("../XMLfiles/WarningTests/test004.xml");
    EXPECT_EQ(3, P.errorOccured(UNKNOWN_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning005) {
    Parser P("../XMLfiles/WarningTests/test005.xml");
    EXPECT_EQ(2, P.errorOccured(MISSING_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning006) {
    Parser P("../XMLfiles/WarningTests/test006.xml");
    EXPECT_EQ(1, P.errorOccured(MISSING_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning007) {
    Parser P("../XMLfiles/WarningTests/test007.xml");
    EXPECT_EQ(2, P.errorOccured(MISSING_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning008) {
    Parser P("../XMLfiles/WarningTests/test008.xml");
    EXPECT_EQ(3, P.errorOccured(MISSING_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning009) {
    Parser P("../XMLfiles/WarningTests/test009.xml");
    EXPECT_EQ(1, P.errorOccured(MISSING_TAG));
}
