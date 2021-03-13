//
// Created by Niels on 6/03/2021.
//

#include "Output.h"

Output::Output(Hub *h, int y, int m, int w, int d) {
    makeOutputFile(h, y, m, w, d);
}

bool Output::isProperlyInitialized() const {
    return _initCheck == this;
}

void Output::makeOutputFile(Hub *h, int y, int m, int w, int d) {
    ofstream outputFile("../Output.txt", ios_base::app);
    outputFile << "Overzicht van vaccinaties na: " << dateToString(y, m, w, d) << string(".\n\n");
    outputFile << "Hub (" << h->getKaantalVaccinsPerLevering() << " vaccins)\n";

    for (map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin();
         it != h->getFverbondenCentra().end(); it++) {
        outputFile << "\t-> " << it->first << " (" << it->second->getAantalVaccinaties() << " vaccins)\n";
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

void Output::makeOutputFile(Hub *h, unsigned int days) {
    unsigned int years = days / 356;
    days -= years * 356;
    unsigned int months = days / 30;
    days -= months * 30;
    unsigned int weeks = days / 7;
    days -= weeks * 7;
    makeOutputFile(h, years, months, weeks, days);
}

string Output::dateToString(unsigned int y, unsigned int m, unsigned int w, unsigned int d) {
    ostringstream ss;
    ss << y << (y == 1 ? " jaar" : " jaren") + string(", ") << m << (m == 1 ? " maand" : " maanden") + string(", ") << w
       << (w == 1 ? " week" : " weken") + string(" en ") << d << (d == 1 ? " dag" : " dagen");
    return ss.str();
}
