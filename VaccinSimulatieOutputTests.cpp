//============================================================================
// Name        : TicTactToeOutputTest.cpp
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

#include "VaccinSimulatie.h"
#include "Lib.h"

class VaccinSimulatieOutputTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        freopen(ERROR_FILE, "a+", stderr);
    }

    virtual void TearDown() {
        fclose(stderr);
    }
};


// Tests the compare files
TEST_F(VaccinSimulatieOutputTest, FileCompare) {
// TicTactToeOutputTest.cpp
// Author      : Serge Demeyer

    ASSERT_TRUE(DirectoryExists(OUTPUT_FILE_LOCATION));

    makeEmptyFile(OUTPUT_FILE_LOCATION + "file1.txt");
    makeEmptyFile(OUTPUT_FILE_LOCATION + "file2.txt");

    ofstream myfile;
    myfile.open((OUTPUT_FILE_LOCATION + "file1.txt").c_str());
    myfile.close();
    myfile.open((OUTPUT_FILE_LOCATION + "file2.txt").c_str());
    myfile.close();

    EXPECT_TRUE(FileExists(OUTPUT_FILE_LOCATION + "file1.txt"));
    EXPECT_TRUE(FileIsEmpty(OUTPUT_FILE_LOCATION + "file1.txt"));
    EXPECT_TRUE(FileExists(OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_TRUE(FileIsEmpty(OUTPUT_FILE_LOCATION + "file2.txt"));


    //compare 2 empty files
// *** Beware: the following does not work with older versions of libstdc++
// *** It doesn't work with gcc version 4.0.1 (Apple Inc. build 5465)
// *** It does work with gcc version 4.2.1
    EXPECT_TRUE(FileCompare(OUTPUT_FILE_LOCATION + "file1.txt", OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_TRUE(FileCompare(OUTPUT_FILE_LOCATION + "file2.txt", OUTPUT_FILE_LOCATION + "file1.txt"));

    //compare an empty and a non-empty files
    myfile.open((OUTPUT_FILE_LOCATION + "file1.txt").c_str());
    myfile << "xxx" << endl << "yyy";
    myfile.close();
    EXPECT_TRUE(FileExists(OUTPUT_FILE_LOCATION + "file1.txt"));
    EXPECT_FALSE(FileIsEmpty(OUTPUT_FILE_LOCATION + "file1.txt"));
    EXPECT_FALSE(FileCompare(OUTPUT_FILE_LOCATION + "file1.txt", OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_FALSE(FileCompare(OUTPUT_FILE_LOCATION + "file2.txt", OUTPUT_FILE_LOCATION + "file1.txt"));

    //compare two equal files
    myfile.open((OUTPUT_FILE_LOCATION + "file2.txt").c_str());
    myfile << "xxx" << endl << "yyy";
    myfile.close();
    EXPECT_TRUE(FileExists(OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_FALSE(FileIsEmpty(OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_TRUE(FileCompare(OUTPUT_FILE_LOCATION + "file1.txt", OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_TRUE(FileCompare(OUTPUT_FILE_LOCATION + "file2.txt", OUTPUT_FILE_LOCATION + "file1.txt"));

    //compare 2 non-empty files which are off by a character in the middle
    myfile.open((OUTPUT_FILE_LOCATION + "file2.txt").c_str());
    myfile << "xxx" << endl << "xyy";
    myfile.close();
    EXPECT_FALSE(FileCompare(OUTPUT_FILE_LOCATION + "file1.txt", OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_FALSE(FileCompare(OUTPUT_FILE_LOCATION + "file2.txt", OUTPUT_FILE_LOCATION + "file1.txt"));

    //compare 2 non-empty files where one is one character shorter than the other
    myfile.open((OUTPUT_FILE_LOCATION + "file2.txt").c_str());
    myfile << "xxx" << endl << "yy";
    myfile.close();
    EXPECT_FALSE(FileCompare(OUTPUT_FILE_LOCATION + "file1.txt", OUTPUT_FILE_LOCATION + "file2.txt"));
    EXPECT_FALSE(FileCompare(OUTPUT_FILE_LOCATION + "file2.txt", OUTPUT_FILE_LOCATION + "file1.txt"));

    //compare existing against non existing file
    EXPECT_FALSE(
            FileCompare(OUTPUT_FILE_LOCATION + "file1.txt", OUTPUT_FILE_LOCATION + "nonexisting.txt"));
    EXPECT_FALSE(
            FileCompare(OUTPUT_FILE_LOCATION + "nonexisting.txt", OUTPUT_FILE_LOCATION + "file1.txt"));
}

/**
Tests the output of a "happy day" scenario (everyone gets vaccinated).
*/
TEST_F(VaccinSimulatieOutputTest, OutputHappyDay) {
    ASSERT_TRUE(DirectoryExists(OUTPUT_FILE_LOCATION));
    //if directory doesn't exist then no need in proceeding with the test

    string testnr = "001";
    int nr = 1;
    string filename = HAPPY_DAY_TESTS_FILE_LOCATION + "test" + testnr + ".xml";
    while (FileExists(filename)) {
        Parser P(filename);
        vector<Hub *> hubs = P.getFhubs();
        std::vector<VaccinatieCentrum *> vaccinatie_centra;

        for (unsigned int i = 0; i < hubs.size(); i++) {
            for (map<string, VaccinatieCentrum *>::const_iterator centrum = hubs[i]->getFverbondenCentra().begin();
                 centrum != hubs[i]->getFverbondenCentra().end(); centrum++) {
                if (find(vaccinatie_centra.begin(), vaccinatie_centra.end(), centrum->second) ==
                    vaccinatie_centra.end()) {
                    vaccinatie_centra.push_back(centrum->second);
                }
            }
        }
        Simulatie(hubs, vaccinatie_centra, filename);
        EXPECT_TRUE(FileExists(HAPPY_DAY_OUTPUT_FILE_LOCATION + "Simpele_Uitvoer_test" + testnr + ".txt"));
        EXPECT_TRUE(FileExists(HAPPY_DAY_OUTPUT_FILE_LOCATION + "Expected_Simpele_Uitvoer_test" + testnr + ".txt"));
        EXPECT_TRUE(FileExists(HAPPY_DAY_OUTPUT_FILE_LOCATION + "Grafische_Impressie_test" + testnr + ".txt"));
        EXPECT_TRUE(FileExists(HAPPY_DAY_OUTPUT_FILE_LOCATION + "Expected_Grafische_Impressie_test" + testnr + ".txt"));

        EXPECT_TRUE(FileCompare(HAPPY_DAY_OUTPUT_FILE_LOCATION + "Simpele_Uitvoer_test" + testnr + ".txt",
                                HAPPY_DAY_OUTPUT_FILE_LOCATION + "Expected_Simpele_Uitvoer_test" + testnr + ".txt"));
        EXPECT_TRUE(FileCompare(HAPPY_DAY_OUTPUT_FILE_LOCATION + "Grafische_Impressie_test" + testnr + ".txt",
                                HAPPY_DAY_OUTPUT_FILE_LOCATION + "Expected_Grafische_Impressie_test" + testnr +
                                ".txt"));

        nr++;
        string new_testnr = to_string(nr);
        while (new_testnr.size() < 3) new_testnr.insert(new_testnr.begin(), '0');
        size_t pos = filename.find(testnr);
        filename.replace(pos, 3, new_testnr);
        testnr = new_testnr;
    }
}

