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
#include "Lib.h"
#include "Parser.h"
#include "Utils.h"
#include "SimulationImporter.h"

using namespace std;


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
        testnr = new_testnr;
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
        testnr = new_testnr;
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
    EXPECT_EQ(2, P.errorOccured(WRONG_VALUE));
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
    EXPECT_EQ(2, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning008) {
    Parser P("../XMLfiles/WarningTests/test008.xml");
    EXPECT_EQ(3, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning009) {
    Parser P("../XMLfiles/WarningTests/test009.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning010) {
    Parser P("../XMLfiles/WarningTests/test010.xml");
    EXPECT_EQ(3, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning011) {
    Parser P("../XMLfiles/WarningTests/test011.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning012) {
    Parser P("../XMLfiles/WarningTests/test012.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning013) {
    Parser P("../XMLfiles/WarningTests/test013.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning014) {
    Parser P("../XMLfiles/WarningTests/test014.xml");
    EXPECT_EQ(4, P.errorOccured(MISSING_TAG));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning015) {
    Parser P("../XMLfiles/WarningTests/test015.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning016) {
    Parser P("../XMLfiles/WarningTests/test016.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning017) {
    Parser P("../XMLfiles/WarningTests/test017.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning018) {
    Parser P("../XMLfiles/WarningTests/test018.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning019) {
    Parser P("../XMLfiles/WarningTests/test019.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, InputXMLWarning020) {
    Parser P("../XMLfiles/WarningTests/test020.xml");
    EXPECT_EQ(1, P.errorOccured(WRONG_VALUE));
}

TEST_F(VaccinSimulatieInputTest, BackupTest001) {
    SimulationImporter importer("../SavedData/TestFiles/BackupTest001.xml");
    EXPECT_TRUE(importer.isProperlyInitialized());

    EXPECT_EQ(0, importer.getTotaalAantalEerstePrikken("Park Spoor Oost"));
    EXPECT_NO_FATAL_FAILURE(importer.getTotaalAantalEerstePrikken("Park Spoor West"));

    EXPECT_EQ(0, importer.getCentrumVaccinCount("Park Spoor Oost", "AstraZeneca"));
    EXPECT_NO_FATAL_FAILURE(importer.getCentrumVaccinCount("Park Spoor Oost", "Pfizer"));
    EXPECT_NO_FATAL_FAILURE(importer.getCentrumVaccinCount("Park Spoor West", "AstraZeneca"));
    EXPECT_NO_FATAL_FAILURE(importer.getCentrumVaccinCount("Park Spoor West", "Pfizer"));

    EXPECT_EQ(0, importer.getHubVaccinCount(0, "AstraZeneca"));
    EXPECT_NO_FATAL_FAILURE(importer.getHubVaccinCount(0, "Moderna"));
    EXPECT_NO_FATAL_FAILURE(importer.getHubVaccinCount(1, "AstraZeneca"));
    EXPECT_NO_FATAL_FAILURE(importer.getHubVaccinCount(1, "Moderna"));

    EXPECT_EQ(0, importer.getTotaalAantalVaccinatiesCentrum("Park Spoor Oost"));
    EXPECT_NO_FATAL_FAILURE(importer.getTotaalAantalVaccinatiesCentrum("Park Spoor West"));

    EXPECT_EQ(0, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "AstraZeneca"));
    EXPECT_NO_FATAL_FAILURE(importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "Moderna"));
    EXPECT_NO_FATAL_FAILURE(importer.getAantalVaccinatiesCentrum("Park Spoor West", "AstraZeneca"));
    EXPECT_NO_FATAL_FAILURE(importer.getAantalVaccinatiesCentrum("Park Spoor West", "Moderna"));
}

TEST_F(VaccinSimulatieInputTest, BackupTest002) {
    SimulationImporter importer("../SavedData/TestFiles/BackupTest002.xml");
    EXPECT_TRUE(importer.isProperlyInitialized());

    EXPECT_EQ(2000, importer.getTotaalAantalEerstePrikken("Park Spoor Oost"));
    EXPECT_EQ(1000, importer.getCentrumVaccinCount("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(500, importer.getHubVaccinCount(0, "AstraZeneca"));
    EXPECT_EQ(3000, importer.getTotaalAantalVaccinatiesCentrum("Park Spoor Oost"));
    EXPECT_EQ(3000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "AstraZeneca"));
}

TEST_F(VaccinSimulatieInputTest, BackupTest003) {
    SimulationImporter importer("../SavedData/TestFiles/BackupTest003.xml");
    EXPECT_TRUE(importer.isProperlyInitialized());

    EXPECT_EQ(7000, importer.getTotaalAantalEerstePrikken("Park Spoor Oost"));
    EXPECT_EQ(1000, importer.getCentrumVaccinCount("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(4000, importer.getCentrumVaccinCount("Park Spoor Oost", "Pfizer"));
    EXPECT_EQ(500, importer.getHubVaccinCount(0, "AstraZeneca"));
    EXPECT_EQ(750, importer.getHubVaccinCount(0, "Pfizer"));
    EXPECT_EQ(9000, importer.getTotaalAantalVaccinatiesCentrum("Park Spoor Oost"));
    EXPECT_EQ(3000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(6000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "Pfizer"));
}

TEST_F(VaccinSimulatieInputTest, BackupTest004) {
    SimulationImporter importer("../SavedData/TestFiles/BackupTest004.xml");
    EXPECT_TRUE(importer.isProperlyInitialized());

    EXPECT_EQ(7000, importer.getTotaalAantalEerstePrikken("Park Spoor Oost"));
    EXPECT_EQ(1000, importer.getCentrumVaccinCount("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(4000, importer.getCentrumVaccinCount("Park Spoor Oost", "Pfizer"));
    EXPECT_EQ(500, importer.getHubVaccinCount(0, "AstraZeneca"));
    EXPECT_EQ(750, importer.getHubVaccinCount(0, "Pfizer"));
    EXPECT_EQ(9000, importer.getTotaalAantalVaccinatiesCentrum("Park Spoor Oost"));
    EXPECT_EQ(3000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(6000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "Pfizer"));

    EXPECT_EQ(7000, importer.getTotaalAantalEerstePrikken("De Zoerla"));
    EXPECT_EQ(6000, importer.getCentrumVaccinCount("De Zoerla", "AstraZeneca"));
    EXPECT_EQ(3000, importer.getCentrumVaccinCount("De Zoerla", "Pfizer"));;
    EXPECT_EQ(5000, importer.getTotaalAantalVaccinatiesCentrum("De Zoerla"));
    EXPECT_EQ(4000, importer.getAantalVaccinatiesCentrum("De Zoerla", "AstraZeneca"));
    EXPECT_EQ(1000, importer.getAantalVaccinatiesCentrum("De Zoerla", "Pfizer"));
}

TEST_F(VaccinSimulatieInputTest, BackupTest005) {
    SimulationImporter importer("../SavedData/TestFiles/BackupTest004.xml");
    EXPECT_TRUE(importer.isProperlyInitialized());

    EXPECT_EQ(7000, importer.getTotaalAantalEerstePrikken("Park Spoor Oost"));
    EXPECT_EQ(1000, importer.getCentrumVaccinCount("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(4000, importer.getCentrumVaccinCount("Park Spoor Oost", "Pfizer"));
    EXPECT_EQ(500, importer.getHubVaccinCount(0, "AstraZeneca"));
    EXPECT_EQ(750, importer.getHubVaccinCount(0, "Pfizer"));
    EXPECT_EQ(9000, importer.getTotaalAantalVaccinatiesCentrum("Park Spoor Oost"));
    EXPECT_EQ(3000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "AstraZeneca"));
    EXPECT_EQ(6000, importer.getAantalVaccinatiesCentrum("Park Spoor Oost", "Pfizer"));

    EXPECT_EQ(7000, importer.getTotaalAantalEerstePrikken("De Zoerla"));
    EXPECT_EQ(6000, importer.getCentrumVaccinCount("De Zoerla", "AstraZeneca"));
    EXPECT_EQ(3000, importer.getCentrumVaccinCount("De Zoerla", "Pfizer"));
    EXPECT_EQ(5000, importer.getTotaalAantalVaccinatiesCentrum("De Zoerla"));
    EXPECT_EQ(4000, importer.getAantalVaccinatiesCentrum("De Zoerla", "AstraZeneca"));
    EXPECT_EQ(1000, importer.getAantalVaccinatiesCentrum("De Zoerla", "Pfizer"));
}