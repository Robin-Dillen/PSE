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
#include <vector>
#include <map>
#include "TinyXML/tinystr.h"
#include "TinyXML/tinyxml.h"

#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "VaccinSimulatie.h"

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
//    REQUIRE(FileExists(leftFileName), "De linkerfile bestaat niet!");
//    REQUIRE(FileExists(rightFileName), "De rechterfile bestaat niet!");
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
    if (FileExists(filename) && FileIsEmpty(filename)) {
        return;
    }
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    ENSURE(FileExists(filename), "Failed creating the file!");
    ENSURE(FileIsEmpty(filename), "Failed emptying the file!");
}
// einde van de gekopieerde functies

/*!
 * zet een address om naar latitude, longitude
 * @param address het address dat wordt omgezet
 * @return std::pair<double, double>
 */
inline std::pair<double, double> addressToCoords(const std::string &address) {
//    kleinste lat: 49.497041, grootste lat: 51.505022
//    kleinste lon: 2.545403, grootste lon: 6.407907
    std::string coords;
    double lat, lon;
    for (int x = 0; x < 3 && coords.empty(); x++) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
        system(("python ../AddressToCoords.py '" + address + "'").c_str());
#else
        system(("python3 ../AddressToCoords.py '" + address + "'").c_str());
#endif
        std::ifstream file("location.txt");
        getline(file, coords);
        std::cout << coords << std::endl;
        size_t delim = coords.find(',');
        lat = std::stod(coords.substr(0, delim - 1));
        lon = std::stod(coords.substr(delim + 1, coords.size()));
    }
    return std::make_pair(lat, lon);
}

/*!
 * exports simulation to XML file
 * @param filename path of the file to be exported to
 * @param simulatie simulation object
 */
inline void ExportSimulation(const std::string &filename, VaccinSimulatie *simulatie) {
    TiXmlDocument doc;
    TiXmlElement *root = new TiXmlElement("root");
    doc.LinkEndChild(root);
    const std::vector<Hub *> &hubs = simulatie->getHubs();
    for (std::vector<Hub *>::const_iterator hubIterator = hubs.begin(); hubIterator != hubs.end(); ++hubIterator) {
        TiXmlElement *hub = new TiXmlElement("HUB");
        map<std::string, Vaccin *> vaccins = (*hubIterator)->getVaccins();
        for (map<std::string, Vaccin *>::const_iterator vaccinIterator = vaccins.begin();
             vaccinIterator != vaccins.end(); ++vaccinIterator) {
            TiXmlElement *vaccin = new TiXmlElement("VACCIN");
            TiXmlElement *type = new TiXmlElement("type");
            TiXmlText *type_value = new TiXmlText(vaccinIterator->first.c_str());
            type->LinkEndChild(type_value);
            TiXmlElement *aantal = new TiXmlElement("aantal");
            TiXmlText *aantal_value = new TiXmlText(
                    to_string((*hubIterator)->getAantalVaccins(vaccinIterator->first)).c_str());
            aantal->LinkEndChild(aantal_value);
            vaccin->LinkEndChild(type);
            vaccin->LinkEndChild(aantal);
            hub->LinkEndChild(vaccin);
        }
        root->LinkEndChild(hub);
    }
    const std::vector<VaccinatieCentrum *> &centra = simulatie->getVaccinatieCentra();
    for (std::vector<VaccinatieCentrum *>::const_iterator centrumIterator = centra.begin();
         centrumIterator != centra.end(); ++centrumIterator) {
        TiXmlElement *centrum = new TiXmlElement("CENTRUM");
        const map<std::string, pair<Vaccin *, int>> &vaccins = (*centrumIterator)->getAantalVaccins1();
        for (map<std::string, pair<Vaccin *, int>>::const_iterator vaccinIterator = vaccins.begin();
             vaccinIterator != vaccins.end(); ++vaccinIterator) {
            TiXmlElement *vaccin = new TiXmlElement("VACCIN");
            TiXmlElement *type = new TiXmlElement("type");
            TiXmlText *type_value = new TiXmlText(vaccinIterator->first.c_str());
            type->LinkEndChild(type_value);
            TiXmlElement *aantal = new TiXmlElement("aantal");
            TiXmlText *aantal_value = new TiXmlText(
                    to_string((*centrumIterator)->getAantalVaccins(vaccinIterator->first)).c_str());
            aantal->LinkEndChild(aantal_value);
            int totaal_eerste = 0;
            const deque<int> &eerste_prikken = (*centrumIterator)->getAantalEerstePrikken().at(vaccinIterator->first);
            for (deque<int>::const_iterator eerste_prik = eerste_prikken.begin();
                 eerste_prik != eerste_prikken.end(); ++eerste_prik) {
                totaal_eerste += *eerste_prik;
            }
            TiXmlElement *eersteprik = new TiXmlElement("1steprik");
            TiXmlText *eersteprik_value = new TiXmlText(to_string(totaal_eerste).c_str());
            eersteprik->LinkEndChild(eersteprik_value);
            TiXmlElement *tweedeprik = new TiXmlElement("2deprik");
            TiXmlText *tweedeprik_value = new TiXmlText(
                    to_string((*centrumIterator)->getAantalVaccinaties(vaccinIterator->first)).c_str());
            tweedeprik->LinkEndChild(tweedeprik_value);
            vaccin->LinkEndChild(type);
            vaccin->LinkEndChild(aantal);
            vaccin->LinkEndChild(eersteprik);
            vaccin->LinkEndChild(tweedeprik);
            centrum->LinkEndChild(vaccin);
        }
        root->LinkEndChild(centrum);
    }
    FILE *f = fopen(filename.c_str(), "w");
    ENSURE(f != NULL, "File didn't open correctly!");
    doc.Print(f);
    std::fclose(f);
}



#endif //PSE_UTILS_H
