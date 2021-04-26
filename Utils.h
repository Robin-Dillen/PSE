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
#include <cstdarg>
#include <limits>

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

inline int min(int num_args, ...) {
    int min_ = numeric_limits<int>::max();
    va_list ap;
    int i;

    va_start(ap, num_args);
    for (i = 0; i < num_args; i++) {
        int n = va_arg(ap, int);
        if (n < min_) min_ = n;
    }
    va_end(ap);

    return min_;
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
//
//    result = true; // files exist and are open; assume equality unless a counterexamples shows up.
//    while (result && leftFile.good() && rightFile.good()) {
//        leftFile.get(leftRead);
//        rightFile.get(rightRead);
//        result = (leftRead == rightRead);
//    };
//    if (result) {
//        // last read was still equal; are we at the end of both files ?
//        result = (!leftFile.good()) && (!rightFile.good());
//    };
//
//    leftFile.close();
//    rightFile.close();
//    return result;

//https://stackoverflow.com/a/48877081/10875953
    std::string lineA;
    std::string lineB;
    getline(leftFile, lineA);
    do {
        // read File2 until match is found
        while (getline(rightFile, lineB)) {
            if (lineA == lineB) {
                break;
            }
        }
        if (lineA != lineB) {
            return false;
        }
        // clear the state of File2 stream
        rightFile.clear();
        rightFile.seekg(0, ios::beg);
    } while (getline(leftFile, lineA));
    return true;
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
