//============================================================================
// Name        : Utils.h
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : utility functions
//============================================================================

#ifndef PSE_UTILS_H
#define PSE_UTILS_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <cstdio>
#include <string>

using namespace std;

// converts a all types supporting << to strings
template<typename AType>
string to_string(AType var) {
    string str; // a variable of str data type

    // using the stringstream class to insert an int and
    // extract a string
    stringstream ss;
    ss << var;
    ss >> str;
    return str;
}

// converts an int to a string
int to_int(const string &s) {
    int ret;
    stringstream ss;
    ss << s;
    ss >> ret;
    return ret;
}

// volgende functies zijn van het TicTacToe voorbeeld
bool DirectoryExists(const std::string dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

bool FileExists(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return false;
    ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }
}

bool FileIsEmpty(const std::string filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

bool FileCompare(const std::string leftFileName, const std::string rightFileName) {
    ifstream leftFile, rightFile;
    char leftRead, rightRead;
    bool result;

    // Open the two files.
    leftFile.open(leftFileName.c_str());
    if (!leftFile.is_open()) {
        return false;
    };
    rightFile.open(rightFileName.c_str());
    if (!rightFile.is_open()) {
        leftFile.close();
        return false;
    };

    result = true; // files exist and are open; assume equality unless a counterexamples shows up.
    while (result && leftFile.good() && rightFile.good()) {
        leftFile.get(leftRead);
        rightFile.get(rightRead);
        result = (leftRead == rightRead);
    };
    if (result) {
        // last read was still equal; are we at the end of both files ?
        result = (!leftFile.good()) && (!rightFile.good());
    };

    leftFile.close();
    rightFile.close();
    return result;
}
// einde van de gekopieerde functies

#endif //PSE_UTILS_H
