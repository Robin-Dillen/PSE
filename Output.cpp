//============================================================================
// Name        : OutputSingleton.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : functions to generate output
//============================================================================

#include "Output.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Utils.h"
#include "Lib.h"

bool OutputSingleton::isProperlyInitialized() const {
    return _initCheck == this;
}

void OutputSingleton::addToOutputFile(Hub *h, int i, int y, int m, int w, int d, const string &filename) {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToOutputFile");
    // voegt content toe aan de output file, het is dus de bedoeling dat je de file opvoorhand leeg maakt
    ofstream outputFile((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), ios_base::app);
    outputFile << "Overzicht van vaccinaties na: " << dateToString(y, m, w, d) << string(".\n\n");
    outputFile << "Hub " << i << " (" << h->getTotaalAantalVaccins() << " vaccins)\n";

    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << "\t-> " << it->first << " (" << it->second->getTotaalAantalVaccins() << " vaccins)\n";
    }
    outputFile << "\n";
    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << it->first << ": " << it->second->getKaantalInwoners() - it->second->getAantalNietVaccinaties()
                   << " gevacineerd, nog "
                   << it->second->getAantalNietVaccinaties()
                   << " inwoners niet gevaccineerd.\n";
    }
    outputFile << "\r";
    outputFile.close();
}

void OutputSingleton::addToOutputFile(Hub *h, int i, int days, const string &filename) {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToOutputFile");
    //datum in juiste vorm zetten
    int years = days / 356;
    days -= years * 356;
    int months = days / 30;
    days -= months * 30;
    int weeks = days / 7;
    days -= weeks * 7;
    addToOutputFile(h, i, years, months, weeks, days, filename);
}

void OutputSingleton::addToGIFile(VaccinatieCentrum *v, const string &filename) {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToGIFile");
    // voegt content toe aan de output file, het is dus de bedoeling dat je de file opvoorhand leeg maakt
    ofstream outputFile((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), ios_base::app);
    outputFile << v->getKfname() << ":\n";
    outputFile << "\t- vaccins\t\t[";
    //Percentage van hoeveel vaccins er aanwezig zijn in cnetrum
    int vaccinpercentage = (int) roundf(((float) v->getTotaalAantalVaccins() * 100) / (float) v->getMaxStock());
    for (int i = 1; i <= 20; i++) {
        if (i <= vaccinpercentage / 5) {
            outputFile << "=";
        } else {
            outputFile << " ";
        }
    }
    outputFile << "] " << vaccinpercentage << "%\n";
    outputFile << "\t- 1ste prik\t\t[";
    //Percentage van hoeveel mensen al een 1ste prik hebben gekregen (maar nog geen 2de prik)
    int aantal1steprik = v->getKaantalInwoners() - (v->getTotaalAantalVaccinaties() + v->getAantalNietVaccinaties());
    int eersteprikpercentage = round((aantal1steprik * 100) / v->getKaantalInwoners());
    for (int i = 1; i <= 20; i++) {
        if (i <= eersteprikpercentage / 5) {
            outputFile << "=";
        } else {
            outputFile << " ";
        }
    }
    outputFile << "] " << eersteprikpercentage << "%\n";
    outputFile << "\t- gevaccineerd\t[";
    //Percentage van hoeveel mensen er volledig gevaccineerd zijn
    int gevaccineerdpercentage = round((v->getTotaalAantalVaccinaties() * 100) / v->getKaantalInwoners());
    for (int i = 1; i <= 20; i++) {
        if (i <= gevaccineerdpercentage / 5) {
            outputFile << "=";
        } else {
            outputFile << " ";
        }
    }
    outputFile << "] " << gevaccineerdpercentage << "%\n";
    map<string, int>::const_iterator it;
    int max = 0;
    for (it = v->getAantalVaccinaties1().begin(); it != v->getAantalVaccinaties1().end(); it++) {
        if ((int) it->first.size() > max) max = it->first.size();
    }
    for (it = v->getAantalVaccinaties1().begin(); it != v->getAantalVaccinaties1().end(); it++) {
        outputFile << "\t\t- " << it->first << "\t";
        int x = 0;
        while ((int) it->first.size() + x <= max) {
            outputFile << "\t";
            x += 4;
        }
        outputFile << "[";
        int typeVaccinPercentage;
        if (gevaccineerdpercentage == 0) {
            typeVaccinPercentage = 0;
        } else {
            typeVaccinPercentage = (it->second * 100) / v->getTotaalAantalVaccinaties();
        }
        for (int i = 1; i <= 20; i++) {
            if (i <= typeVaccinPercentage / 5) {
                outputFile << "=";
            } else {
                outputFile << " ";
            }
        }
        outputFile << "] " << typeVaccinPercentage << "%\n";
    }
    outputFile << "\r";
    outputFile.close();
}


void OutputSingleton::addDateToFile(int days, const string &filename) const {
    REQUIRE(isProperlyInitialized(), "Object wasn't initialized when calling addToGIFile");
    //datum in juiste vorm zetten
    int years = days / 356;
    days -= years * 356;
    int months = days / 30;
    days -= months * 30;
    int weeks = days / 7;
    days -= weeks * 7;
    ofstream outputFile((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), ios_base::app);
    outputFile << "Overzicht van vaccinaties na: " << dateToString(years, months, weeks, days) << string(".\n\n");
    outputFile.close();
}

string OutputSingleton::dateToString(int y, int m, int w, int d) {
    ostringstream ss;
    ss << y << (y == 1 ? " jaar" : " jaren") + string(", ") << m << (m == 1 ? " maand" : " maanden") + string(", ") << w
       << (w == 1 ? " week" : " weken") + string(" en ") << d << (d == 1 ? " dag" : " dagen");
    return ss.str();
}

void OutputSingleton::makeOutputFile(const string &filename) {
    // maakt een output file of maakt een outputfile leeg
    makeEmptyFile(filename);
}
