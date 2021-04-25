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
    friend class TicTacToe;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


// Tests the compare files
TEST_F(VaccinSimulatieOutputTest, FileCompare) {
// TicTactToeOutputTest.cpp
// Author      : Serge Demeyer

    ASSERT_TRUE(DirectoryExists(OUTPUT_FILE_LOCATION));

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

        EXPECT_TRUE(FileCompare(OUTPUT_FILE_LOCATION + "Simpele_Uitvoer_test" + testnr + ".txt",
                                OUTPUT_FILE_LOCATION + "Expected_Simpele_Uitvoer_test" + testnr + ".txt"));
        EXPECT_TRUE(FileCompare(OUTPUT_FILE_LOCATION + "Grafische_Impressietest_test" + testnr + ".txt",
                                OUTPUT_FILE_LOCATION + "Expected_Grafische_Impressietest_test" + testnr + ".txt"));

        nr++;
        string new_testnr = to_string(nr);
        while (new_testnr.size() < 3) new_testnr.insert(new_testnr.begin(), '0');
        size_t pos = filename.find(testnr);
        filename.replace(pos, 3, new_testnr);
    }
}

//void auxTestOutput (TicTacToe &game, const std::string expectedOutputFilename) {
//    TicTacToeExporter plainExporter;
//    TicTacToeHTMLExporter htmlExporter;
//    TicTacToeHTMLTablesIconExporter html2Exporter;
//    ofstream myfile;
//
//    myfile.open((OUTPUT_FILE_LOCATION + "/zzzOut.txt").c_str());
//    plainExporter.documentStart(myfile);
//    plainExporter.exportOn(myfile, game);
//    plainExporter.documentEnd(myfile);
//    myfile.close();
//    EXPECT_TRUE(FileCompare(
//            "Outputfiles/zzzOut.txt",
//            "Outputfiles/" + expectedOutputFilename + ".txt"));
//
//    myfile.open("Outputfiles/zzzOut.html");
//    htmlExporter.documentStart(myfile);
//    htmlExporter.exportOn(myfile, game);
//    htmlExporter.documentEnd(myfile);
//    myfile.close();
//    EXPECT_TRUE(FileCompare(
//            "Outputfiles/zzzOut.html",
//            "Outputfiles/" + expectedOutputFilename + ".html"));
//
//    myfile.open("Outputfiles/zzzOut2.html");
//    html2Exporter.documentStart(myfile);
//    html2Exporter.exportOn(myfile, game);
//    html2Exporter.documentEnd(myfile);
//    myfile.close();
//    EXPECT_TRUE(FileCompare(
//            "Outputfiles/zzzOut2.html",
//            "Outputfiles/" + expectedOutputFilename + "2.html"));
//}
//
//
//
///**
//Test the exporter with a series of scenarios.
//*/
//TEST_F(TicTactToeOutputTest, ExporterTests) {
//    ASSERT_TRUE(DirectoryExists("Outputfiles"));
//    //if directory doesn't exist then no need in proceeding with the test
//
//    ttt_.reset();
//    auxTestOutput(ttt_, "exportBlank");
//    ttt_.setMoves("b2c1a2c3b3", "b1a3c2a1");
//    while (ttt_.notDone()) {ttt_.doMove();};
//    auxTestOutput(ttt_, "exportNoWinner");
//    ttt_.reset();
//    ttt_.setMoves("a1b2c3", "");
//    while (ttt_.notDone()) {ttt_.doMove();};
//    auxTestOutput(ttt_, "exportDiagonal");
//}
//
//
//
///**
//Test whether the contracts for the exporter throw exceptions.
//*/
//TEST_F(TicTactToeOutputTest, ExporterTestsContractViolations) {
//    TicTacToeExporter plainExporter;
//    ofstream myfile;
//
//    myfile.open("Outputfiles/zzzOut.txt");
//    EXPECT_TRUE(plainExporter.properlyInitialized());
//    EXPECT_FALSE(plainExporter.documentStarted());
//    EXPECT_DEATH(plainExporter.exportOn(myfile, ttt_), "TicTacToeExporter wasn't in documentStarted when calling exportOn.");
//    plainExporter.documentStart(myfile);
//    EXPECT_TRUE(plainExporter.documentStarted());
//    plainExporter.exportOn(myfile, ttt_);
//    plainExporter.documentEnd(myfile);
//    EXPECT_FALSE(plainExporter.documentStarted());
//    EXPECT_DEATH(plainExporter.exportOn(myfile, ttt_), "TicTacToeExporter wasn't in documentStarted when calling exportOn.");
//    myfile.close();
//}

