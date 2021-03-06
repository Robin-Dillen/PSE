//
// Created by Niels on 6/03/2021.
//

#include "Output.h"
#include "VaccinatieCentrum.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Output::Output(Hub* h, int y, int m, int w) {
    makeOutputFile(h,y,m,w);
}

void Output::makeOutputFile(Hub* h, int y, int m, int w) {
    ofstream outputFile("Output.txt");
    outputFile<<"Overzicht van vaccinaties na: "<<y<<" jaren, "<<m<<" maanden en "<<w<<" weken.\n\n";
    outputFile<<"Hub ("<<h->getKaantalVaccinsPerLevering() <<" vaccins)\n";

    for(map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin(); it!= h->getFverbondenCentra().end(); it++){
        outputFile<<"\t-> "<<it->first<<" ("<< it->second->getAantalVaccinaties()<<" vaccins)\n";
    }
    outputFile<<"\n";
    for(map<string, VaccinatieCentrum *>::const_iterator it = h->getFverbondenCentra().begin(); it!= h->getFverbondenCentra().end(); it++){
        outputFile<<it->first<<": "<< it->second->getAantalVaccinaties()<<" gevacineerd, nog "<< it->second->getKaantalInwoners()-it->second->getAantalVaccinaties()<<" inwoners niet gevaccineerd.\n";
    }
    outputFile.close();
}