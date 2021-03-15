//
// Created by Niels on 6/03/2021.
//

#include "Output.h"
#include "Lib.h"

bool Output::isProperlyInitialized() const {
    return _initCheck == this;
}

void Output::addToOutputFile(Hub *h, int y, int m, int w, int d, const string &filename) {
    // voegt content toe aan de output file, het is dus de bedoeling dat je de file opvoorhand leeg maakt
    ofstream outputFile((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), ios_base::app);
    outputFile << "Overzicht van vaccinaties na: " << dateToString(y, m, w, d) << string(".\n\n");
    outputFile << "Hub (" << h->getAantalVaccins() << " vaccins)\n";

    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << "\t-> " << it->first << " (" << it->second->getAantalGeleverdeVaccinsBuffer() << " vaccins)\n";
    }
    outputFile << "\n";
    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << it->first << ": " << it->second->getAantalVaccinaties() << " gevacineerd, nog "
                   << it->second->getKaantalInwoners() - it->second->getAantalVaccinaties()
                   << " inwoners niet gevaccineerd.\n";
    }
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

string Output::dateToString(int y, int m, int w, int d) {
    ostringstream ss;
    ss << y << (y == 1 ? " jaar" : " jaren") + string(", ") << m << (m == 1 ? " maand" : " maanden") + string(", ") << w
       << (w == 1 ? " week" : " weken") + string(" en ") << d << (d == 1 ? " dag" : " dagen");
    return ss.str();
}

void Output::makeOutputFile(const string &filename) {
    // maakt een output file of maakt een outputfile leeg
    std::ofstream ofs;
    ofs.open((OUTPUT_FILE_LOCATION + filename + ".txt").c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}
