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

}

TEST_F(VaccinSimulatieInputTest, InputXMLSyntaxErrors) {
}

/*!
 * tests wether too many tags were give, or when an unknown element is read
 */
TEST_F(VaccinSimulatieInputTest, InputXMLWarnings) {
}

