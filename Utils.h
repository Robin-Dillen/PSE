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
inline string to_string(AType var) {
    string str; // a variable of str data type

    // using the stringstream class to insert an int and
    // extract a string
    stringstream ss;
    ss << var;
    ss >> str;
    return str;
}

// converts an string to an int
inline int to_int(const string &s) {
    int ret;
    stringstream ss;
    ss << s;
    ss >> ret;
    return ret;
}

// volgende functies zijn van het TicTacToe voorbeeld
inline bool DirectoryExists(const std::string &dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

inline bool FileExists(const std::string &filename) {
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

inline bool FileIsEmpty(const std::string &filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
    return st.st_size == 0;
}

inline bool FileCompare(const std::string &leftFileName, const std::string &rightFileName) {
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

/**
 * clears a file, if it exists, doesn't create a new file
 * @param filename Full path to the file with extension!
 * ENSURE(FileIsEmpty(filename), "Failed emptying the file!");
 */
inline void emptyFile(const string &filename) {
    if (!FileExists(filename) || FileIsEmpty(filename)) {
        return;
    }
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ENSURE(FileIsEmpty(filename), "Failed emptying the file!");
}

/**
 * clears a file, or creates one if it doesn't exist
 * @param filename Full path to the file with extension!
 * ENSURE(FileExists(filename), "Failed creating the file!");
 * ENSURE(FileIsEmpty(filename), "Failed emptying the file!");
 */
inline void makeEmptyFile(const string &filename) {
    if (FileIsEmpty(filename)) {
        return;
    }
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ENSURE(FileExists(filename), "Failed creating the file!");
    ENSURE(FileIsEmpty(filename), "Failed emptying the file!");
}
// einde van de gekopieerde functies

#endif //PSE_UTILS_H
