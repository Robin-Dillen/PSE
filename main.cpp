//============================================================================
// Name        : main.cpp
// Author      : Niels Van den Broeck, Robin Dillen
// Version     :
// Copyright   : Project Software Engineering - BA1 Informatica - Niels Van den Broeck, Robin Dillen - University of Antwerp
// Description : ...
//============================================================================

#include <iostream>
#include <vector>

#include "Parser.h"
#include "VaccinatieSimulatie.h"

using namespace std;

int main(int argc, char const *argv[]) {
    vector<string> args = vector<string>(argv + 1, argv + argc);
    //Parser P(args[0]);
    Hub H(93000, 6, 2000);
    vector<VaccinatieCentrum *> centra;
    centra.push_back(new VaccinatieCentrum(7500, 540173, "Park Spoor Oost", "Noordersingel 40, Antwerpen"));
    centra.push_back(new VaccinatieCentrum(2000, 76935, "AED Studios", "Fabriekstraat 38, Lint"));
    H.setFverbondenCentra(centra);
    VaccinatieSimulatie sim(&H);
    sim.start();

    return 0;
}

