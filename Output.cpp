//============================================================================
// Name        : Output.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     : 1.0
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : functions to generate output
//============================================================================

#include "Output.h"
#include "Hub.h"
#include "VaccinatieCentrum.h"
#include "Utils.h"

bool Output::isProperlyInitialized() const {
    return _initCheck == this;
}

void Output::addToOutputFile(Hub *h, int y, int m, int w, int d, const string &filename) {
    // voegt content toe aan de output file, het is dus de bedoeling dat je de file opvoorhand leeg maakt
    ofstream outputFile((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), ios_base::app);
    outputFile << "Overzicht van vaccinaties na: " << dateToString(y, m, w, d) << string(".\n\n");
    outputFile << "Hub (" << h->getTotaalAantalvaccins() << " vaccins)\n";

    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << "\t-> " << it->first << " (" << it->second->getTotaalAantalVaccins() << " vaccins)\n";
    }
    outputFile << "\n";
    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << it->first << ": " << it->second->getTotaalAantalVaccinaties() << " gevacineerd, nog "
                   << it->second->getKaantalInwoners() - it->second->getTotaalAantalVaccinaties()
                   << " inwoners niet gevaccineerd.\n";
    }
    outputFile << "\r";
    outputFile.close();
}

void Output::addToOutputFile(VaccinatieCentrum *v, int y, int m, int w, int d, const string &filename) {
    // voegt content toe aan de output file, het is dus de bedoeling dat je de file opvoorhand leeg maakt
    ofstream outputFile((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), ios_base::app);
    outputFile << "Overzicht van vaccinaties na: " << dateToString(y, m, w, d) << string(".\n\n");
    outputFile << v->getKfname() << ":\n";
    outputFile << "\t- vaccins\t\t[";
    int vaccinpercentage = round((v->getTotaalAantalVaccins()*100)/v->getMaxStock());
    for(int i = 1; i<=20; i++){
        if(i<vaccinpercentage/5){
            outputFile << "=";
        }
        else{
            outputFile << " ";
        }
    }
    outputFile <<" "<<vaccinpercentage<< "%\n";
    outputFile << "\t- gevaccineerd\t[";
    int gevaccineerdpercentage = round((v->getTotaalAantalVaccinaties()*100)/v->getKaantalInwoners());
    for(int i = 1; i<=20; i++){
        if(i<gevaccineerdpercentage/5){
            outputFile << "=";
        }
        else{
            outputFile << " ";
        }
    }
    outputFile <<" "<<gevaccineerdpercentage<< "%\n";
    outputFile << "\r";
    outputFile.close();
}

void Output::addToOutputFile(Hub *h, int days, const string &filename) {
    int years = days / 356;
    days -= years * 356;
    int months = days / 30;
    days -= months * 30;
    int weeks = days / 7;
    days -= weeks * 7;
    addToOutputFile(h, years, months, weeks, days, filename);
}


void Output::addToOutputFile(VaccinatieCentrum *v, int days, const string &filename) {
    int years = days / 356;
    days -= years * 356;
    int months = days / 30;
    days -= months * 30;
    int weeks = days / 7;
    days -= weeks * 7;
    addToOutputFile(v, years, months, weeks, days, filename);
}

string Output::dateToString(int y, int m, int w, int d) {
    ostringstream ss;
    ss << y << (y == 1 ? " jaar" : " jaren") + string(", ") << m << (m == 1 ? " maand" : " maanden") + string(", ") << w
       << (w == 1 ? " week" : " weken") + string(" en ") << d << (d == 1 ? " dag" : " dagen");
    return ss.str();
}

void Output::makeOutputFile(const string &filename) {
    // maakt een output file of maakt een outputfile leeg
    makeEmptyFile(OUTPUT_FILE_LOCATION + filename + ".txt");
}
